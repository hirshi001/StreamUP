# Weighted Priority
Weighted Priority for each channel is defined as assigning each channel a weight, ***w***. When all channels cannot be starved ***w*** is the amount of bytes each channel can send at a given time step. In implementation we use Deficit Round Robin, modified to account for stream vs message based channels.

In this implementation, we maintain the invariance that channel deficit <= channel size.

Updating the deficit:
1. When updating the deficit, all channels must have their deficit increased
2. We should only update the deficit of channel if any of the following are true:
	1. No channel can send because:
		1. Stream channels have 0 deficit
		2. All message channels do not have enough deficit to queue a message
	2. The sum of the deficit of all channels is less than the congestion window

Channel::getNextMessage(int) -> ArrayView<uint8_t>
- Returns none if there is no next message
- Returns the next message that should be considered based on the channel reliability, ordering, and the provided parameter representing the deficit size.
- If the there is no message which has a smaller size than the deficit parameter size, it will return the next best message that would be sent if the deficit parameter were to increase to a minimally satisfying value
Channel::canSend() -> std::optional<ArrayView<uint8_t>>
- if the channel is a message channel and non incremental: returns the value of Channel::getNextMessage(deficit) if it exists and said message's size is greater than or equal to the deficit, otherwise returns std::nullopt. 
- If the channel is a (reliable message channel and incremental) or (stream channel): always return the min(size(), deficit), or std::nullopt if such value is 0

```C++
int cWnd = ...;
final int headerSize = 4; // The number of bytes it takes to include an addition channel of data into a packet
int PMTU = 1200; // This value can change throughout the connection
final int quantumScale = 100;
auto buffer = createBuffer();

void sendLoop()
{
	performRound();
}

void performRound() 
{
	if(shouldUpdateDeficit()) {
		updateDeficit();
	}
	queueFrames(); // poll from the channels to create frames which should be sent
	combineFrames(); // look at all the queued/sent frames, and combine if possible
	send();
}

bool shouldUpdateDeficit() 
{
	bool canSend = false;
	uint64_t totalDeficit = 0;
	for(Channel c : channels) // Sort by highest channel weight first
	{
		totalDeficit += c.deficit;
		if(!canSend)
		{
			if(c.canSend()) 
			{
				canSend = true;
			}
		}
	}
	return !canSend || totalDeficit < cWnd;
}

void updateDeficit() 
{
	// first check if we should update the deficit
	for(Channel c : channels) // Sort by highest channel weight first
	{
		if(c.size() == 0) // only go through active channels
			continue;
		
		c.deficit += c.weight * quantumScale;
		c.deficit = min(c.deficit, c.size());
	}
}
	
```

```C++
void queueFrames() {
	for(Channel c : channels.sorted()) // Sort by highest channel weight first
	{
		if(!c.canSend()) // only go through active channels
			continue;
			
		// Always write if there are more the PMTU bytes to write in the channel
		bool shouldWrite;
		int toWrite;
		
		if(c.deficit >= PMTU - headerSize) 
		{
			shouldWrite = true;
		}

		// check for other cases of when to write based on channel options
		if(!shouldWrite) {
			if(c.isStreamChannel()) 
			{
				 shouldWrite = c.isClosed();
				 toWrite = c.size();
			} 
			else // if(c.isMessageChannel())
			{
				while(true) 
				{
				
				}
			}
		}
			
	}
}
```
## Starvability
The question is, how do we handle weighted priority when channels are allowed to be starved?