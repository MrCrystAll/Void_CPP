# Reward pseudo algorithm

## Before starting
```
set reward to 0.0f;
set ballDir to normalized ball vel;
set agentDir to normalized player vel;
set targetDir to -1 if ballDir x < 0 else 1
```

## Ground to wall

```
if abs(ball x) < wall_x - distToWallThreshold and player height < groundThresh + 17 and ball height < groundThresh + BALL_RADIUS
    set agentToBall to ball pos - agent pos
    set agentToBallDist to agentToBall magnitude
    
    if agentToBallDist < agentDistToBallThresh
	    if abs(agentToBall.x) < ballOffsetX and abs(agentToBall.y) < ballOffsetY
		    set reward to reward + behindTheBallReward

    set speedDiff to max(0.01f, abs(dist(ball vel, agent vel)) / SUPERSONIC_THRESHOLD)

    set directionSimilarity to dot(agentDir, ballDir)

    set speedMatchingBonus to 1 / pow(speedDiff, 0.1f) * speedMatchW;

    set ballDistPunish to dist(ball pos, agent pos) / ballDistReduction

    if dot(agent vel, (targetDir, 0, 0)) > wallAgentAndBallThreshold and dot(ball vel, (targetDir, 0, 0)) > wallAgentAndBallThreshold
        set reward to reward + wallAgentAndBallPunishment

    set directionMatchBonus to similarityBallAgentReward if directionSimilarity >= similarityBallAgentThresh else 0;

    set reward to reward + speedDiff
    set reward to reward + directionSimilarity
    set reward to reward + speedMatchingBonus
    set reward to reward + ballDistPunish
    set reward to reward + directionMatchBonus
    
    if player touch ball:
        set reward to touchReward

```