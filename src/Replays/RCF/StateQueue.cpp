#include <Replays/RCF/StateQueue.h>

USE_RCF_NS;

void StateQueue::PushToQueue(const GameState state)
{
	if (this->queue.size() >= this->queueSize) this->queue.pop();
	this->queue.push(state);
}

const GameState StateQueue::GetFront() const
{
	return this->queue.front();
}

void StateQueue::ClearQueue()
{
	while (not this->queue.empty()) this->queue.pop();
}

const bool StateQueue::IsFull()
{
	return this->queue.size() == this->queueSize;
}
