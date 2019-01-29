# Conector de TinyDB a SNMP
# Temperatura y Humedad

from datetime import datetime, timedelta
from tinydb import TinyDB, Query
db = TinyDB('/opt/MQTT/sensores.json')
Sensores = Query()

search=db.get(Sensores.dev_id == 'sensor-05-caida')

def TTN2time(search):
    """
    Convierte el formato de hora de TTN, a algo usable con datetime
    """
    timeTTN = datetime.strptime((search.get("time")[0:10] + " " + search.get("time")[11:19]), "%Y-%m-%d %H:%M:%S")
    return timeTTN

# Medimos el tiempo según TTN, el tiempo de hace 5 minutos. Si no hemos recibido datos, el sensor responde "0"
timeTTN=TTN2time(search)
timeFail=datetime.utcnow()
# Debug
#print (timeFail - timedelta(minutes=5), timeTTN)

print('caida')
if (timeFail - timedelta(minutes=5) < timeTTN):
    # 0 - sí hay corriente eléctrica. 1 - no hay corriente eléctrica, caída.
    print(search.get("caida"))
else:
    # 2 - no se está recibiendo información reciente
    print("2")

