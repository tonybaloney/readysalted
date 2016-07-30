import paho.mqtt.publish as publish
import paho.mqtt.client as mqtt

topic = "fountainStateOff"
server = "168.128.36.204"

publish.single(
    topic, payload=None, qos=0, retain=False, hostname=server,
    port=1883, client_id="cabbage_client", keepalive=60, will=None, auth=None, tls=None,
    protocol=mqtt.MQTTv31)
