# readysalted
An Internet of Things toolkit for SaltStack

# Why?

Well, I have some home-automation goals I'd like to achieve (first off).

I've reviewed the following options:

- Commercial products, typically Belkin, expensive, badly secured, the apps are terrible, they have no APIs.
- Octoblu - aka Skynet - was a big fan of this project, since their acquisition they've gone very quiet.

I think SaltStack would be an amazin fit for IoT because

- **Scale** - It is already setup for lots of minions, it has an inbuilt targeting system which would suit IoT
- **States** - The idea of applying states e.g. "off" or "unlocked" to a device makes a lot of sense
- **Events** - Not only applying states or sending messages to remote devices but also receiving and acting upon events would be sweet, for example,
the Salt master could be receiving events to say that the bedrooms are too hot or the doorbell has been pressed. Then I can configure what I want to do
with that using reactors.
- **Proxy** - There is already an idea of proxy minions
- **Ecosystem** - IoT is not just about sending and receiving data from sensors, it's about getting events from other systems and doing something about it, so being
able to setup reactors to respond to an event on Device A and set the state of Device B would create a wonderful home automation nirvana!

# What?

## Target hardware

I'm ruling the Raspberry Pi out, they're too expensive and bloated for tiny sensors, I would use one of

- Arduino Uno, Mega or LilyPad
- The ESP8266 https://www.sparkfun.com/products/13678

# Possible approaches

I have come up with a few options to achieve this

## Option 1 - MQTT using an execution module, a proxy minion and a state module

This idea is to implement [MQTT](https://en.wikipedia.org/wiki/MQTT) as the transport mechanism and then each piece of hardware has custom firmware to respond to particular events.

The implementation would be-

- An MQTT execution module, this would allow you to send "one off" events like "disable lock"
- A beacon module for listening (subscribing) to an MQTT broker on specific topics and raising internal events on the Salt bus for them
- A state module (maybe?) for setting states of specific devices, this would be to combine the payload and the topic, e.g. topic "setLightingState" and payload "1" or "0".

### Pros

- MQTT is fairly standard and reliable as a messaging protocol
- This works nicely with salt already and inside my expertise levels (ish)

### Cons

- Requires seperate MQTT server- mosquitto is a great fit, free and open-source, but still its another thing to babysit
- Not hugely flexible, requires reflashing the firmware each time

### Example

- See [mqtt_fountain](mqtt/mqtt_fountain) for the example arduino program I wrote to control the pump of the fountain.
- See [salt@mqtt](mqtt/salt) for the Salt branch I'm maintaining with the mqtt modules.

## Option 2 - Port the Salt minion Micropython

So the ESP8266 runs a port of Python- micropython and they've ported a lot of the standard library. So basically build a minion on micropython.

The idea for this implementation is

- Test and develop the salt minion to work on Micropython

### Pros

- This would be really cool, much hackernews points.

### Cons

- Only works on ESP8266
- I have no idea if this will work and the time I will likely find out it wont will be after months of effort.
- I suspect libzmq will be a real challenge

## Option 3 - Run a ZeroMQ client on the device

Instead of writing all these crazy proxies, I write a "microminion" in C which talks to the master's ZeroMQ server natively. Therefore
it can send and receive events directly from the Salt bus.

### Pros

- Would be really flexible and high-performance

### Cons

- Not sure if ZeroMQ is designed for devices (i.e. terrible connections)
- No idea how the Salt implementation of ZMQ works
- Security? 

## Option 4 - Write a MQTT transport module for SaltStack

I've seen the RAET transport module for Salt, so perhaps one could be written for MQTT, instead of sending and receiving events to Mosquitto,
they get sent directly to the salt master.

### Pros

- Performance would be better than via mosquitto
- Salt could now be used for crappy-connection minions

### Cons

- This makes sense in my head, but I need to check with the salt team
- Probably quite a bit of work to implement

## Option 5 - ??