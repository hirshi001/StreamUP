# Stream vs Message
- Some channels are stream based, while others are message based. This is to help determine when to send bytes from a given channel, since we assume incomplete messages are useless.
## Message
### Reliability
- Reliable (Sequenced/Unsequenced)
#reliability #reliable
#### Unreliable
- Unreliable Sequenced
	- drops out of order messages
- Unreliable Unsequenced
	- Out of order messages 
- Latest Message Reliability
	- Makes best effort attempt to ensure only latest message requested to be sent arrives at receiving side
	- If sender sends message 1, then sends message 2, only tries to ensure that message 2 gets received. Likewise, if message 3 is sent after message 2, the sender only tries to ensure the reliability of message 3, and not of message 1 or 2
	- Use case examples:
		- Infrequent state updates
#unreliable #unreliable-sequenced #unreliable-unsequenced #latest-message-reliability
## Stream
- All stream channels are reliable
#stream #message
# Starvability 
- Starvable (Must be explicitly enabled)
- Unstarvable (Default)
#starvability #starvable #unstarvable
# Priority
- Weighting? <-- Better
- Or just Priority Levels?
#priority #priority-weighting

[[Weighted Priority with Starvability Option]]
# Flow Control
## Pause Sender vs Drop Old Packets
If the receiving application cannot consume the data in a channel fast enough, it has some options based on the channel type.
1. Reliable Message and Stream Channels
	- The sender must stop sending when there is not enough flow control available
2. Unreliable Message
	1.  SENDER_WAIT: The sender can stop sending until more flow control is available
	or
	2. MESSAGE_OVERWRITE: New messages overwrite old messages

#flow-control #SENDER_WAIT #MESSAGE_OVERWRITE


#channel-parameters