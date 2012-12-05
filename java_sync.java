import java.util.LinkedList;
import java.util.Queue;
import java.util.concurrent.Semaphore;


public class java_sync {

	Semaphore intersection[];
	Queue<Thread>[] queues;
	int ended = 0;

	public java_sync(int number_of_cars) throws InterruptedException{
		//Initiate Varibales
		intersection = new Semaphore[4];
		queues = new Queue[4];
		for (int i = 0; i < queues.length; i++){
			queues[i] = new LinkedList<Thread>();
			intersection[i] = new Semaphore(0);
		}
		
		for (int i = 0; i < number_of_cars; i++){
			arrive((int) (rndom()*4));
		}
		
		for (int i = 0; i < queues.length; i++){
			drive(i);
		}
		intersection[0].release();

	}
	
	private void arrive(final int que) {
		queues[que].add(new Thread(new Runnable() {
			
			@Override
			public void run() {
				try {
					intersection[que].acquire(); //Get semaphore
					queues[que].poll();
					Thread.sleep(10); //simulate driving
					drive(que);
					int nextQue = nextQue(que);
					intersection[nextQue].release(); //Release next semaphore
					printQues();
				} catch (InterruptedException e) {
					//Do Nothing
				}
				
			}
		}));
		
	}
	
	public int nextQue(int que){
		boolean stop = true;
		int i = 0;
		while (stop && i < queues.length){
			if (!queues[i++].isEmpty()) {
				stop = false;
			}
		}
		if (stop){
			return 0;
		}
		int nextQue = que+1;
		if (nextQue == queues.length) nextQue = 0;
		while (queues[nextQue].isEmpty()){
			nextQue = nextQue+1;
			if (nextQue == queues.length) nextQue = 0;
		}
		return nextQue;
	}

	// Creates new thread and reducdes that que by one
	public void drive(int que) throws InterruptedException{
		if (!queues[que].isEmpty()){
			Thread car = queues[que].peek();

			car.start();
			
		}
	}
	
	private void printQues() {
		System.out.println("--------------------");
		System.out.println("N: " + queues[0].size());
		System.out.println("E: " + queues[1].size());
		System.out.println("S: " + queues[2].size());
		System.out.println("W: " + queues[3].size());
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
