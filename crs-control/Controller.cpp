

Class Controller
{
	
	private int mode;
	
	public Controller()
	{
	
	}
	
	private int currentPosition;
	public int GetCurrentPosition
	{
		return currentPosition;
	}
	public void SetCurrentPosition(int value)
	{
		currentPosition = value;
	}
	
	private float targetPosition;
	public void SetTargetPosition(float value)
	{
		targetPosition = value;
	}

	private int maxSpeed;
	public int GetMaxSpeed
	{
		return maxSpeed;
	}
	public void SetMaxSpeed(int value)
	{
		maxSpeed = value;
	}
		
	private int maxAcceleration;
	public int GetMaxAcceleration
	{
		return maxAcceleration;
	}
	public void SetMaxAcceleration(int value)
	{
		maxAcceleration = value;
	}
		
	private int maxAbsolutePosition;
	public int GetMaxAbsolutePosition
	{
		return maxAbsolutePosition;
	}
	public void SetMaxAbsolutePosition(int value)
	{
		maxAbsolutePosition = value;
	}

	private int minAbsolutePosition;
	public int GetMinAbsolutePosition
	{
		return minAbsolutePosition;
	}
	public void SetMinAbsolutePosition(int value)
	{
		minAbsolutePosition = value;
	}
	
	public void StartRotatingLeft()
	{
		
	}
	
	public void StartRotatingRight()
	{
		
	}
	
	public void StopRotaing()
	{
		
	}

}
