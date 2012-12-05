import java.util.concurrent.Semaphore;


public class java_sync {

	Semaphore intersection;
	int[] queues;

	public java_sync(int number_of_cars) throws InterruptedException{
		intersection = new Semaphore(1);
		queues = new int[4];
		
		for (int i = 0; i < number_of_cars; i++){
			queues[(int) (rndom()*4)]++;
		}
		long currentIteration = 0;
		while (queues[0] != 0 || queues[1] != 0 || queues[2] != 0 || queues[3] != 0){
			drive((int) (currentIteration % queues.length));
			
			currentIteration++;
		}
	}
	
	// Creates new thread and reducdes that que by one
	public void drive(int que) throws InterruptedException{
		if (queues[que] > 0){
			intersection.acquire();
			Thread car = new Thread(new Runnable() {
				
				@Override
				public void run() {
					
					try {
						Thread.sleep(10);
					} catch (InterruptedException e) {
						//do nothing
					}
					intersection.release();
					
				}
			});
			car.start();
			queues[que]--;
			printQues();
		}
	}
	
	private void printQues() {
		System.out.println("--------------------");
		System.out.println("N: " + queues[0]);
		System.out.println("E: " + queues[1]);
		System.out.println("S: " + queues[2]);
		System.out.println("W: " + queues[3]);
	}

	public static void main(String[] args) {
		if (args.length != 1) {
			System.out.println("Incorrect number of args");
			System.exit(1);
		}
		
		int number_of_cars = Integer.parseInt(args[0]);
		
		try{
		
		java_sync intersec = new java_sync(number_of_cars);
		
		} catch(Exception e){
			//do nothing 
		}
		
		
		

	}
	static long state = 1;
	/* Retruns a random number from 0 to 1 */
	private static double rndom(){
		long A = 48271;
		long M = 2147483647;
		long Q = M/A;
		long R = M%A;
		
		
		long t = A * (state % Q) - R * (state / Q);
		if (t > 0){
			state = t;
		} else {
			state = t + M;
		}
		return ((double) state/M);
		
	}

}
