#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>


//prints the result of primes for a quarter of range
void printQuarterResult(int start, int end, int count, long long sum){
	pid_t pid = getpid();
	pid_t ppid = getppid();

	printf("pid: %d, ppid %d - Count and sum of prime numbers between %d and %d are %d and %lld\n",
			pid, ppid, start, end, count, sum);
}

//calcuates the number of primes within the given range and fines count and sum
//Logic for this function was found on this website: https://www.programiz.com/c-programming/examples/prime-number-intervals
void calculatePrime(int start, int end, int *count, long long *sum){
	*count = 0; 
	*sum = 0;
	int flag = 0;

	while(start < end){
		flag = 0;
	
		if(start <= 1){
			++start;
			continue;
		}

		for (int i = 2; i <= start / 2; ++i){
			if(start % i ==0){
				flag = 1;
				break;
			}
		}
			
		if(flag == 0){
			(*sum) += start;
			(*count)++;
		}

		start++;
		}
}

int main(int argc, char *argv[]){
	if(argc < 4){
		printf("Usage: %s <mode> <min> <max>\n", argv[0]);
		return 1;
	}

	//Checks if the given inputs are positive integers, used the logic I found on claude: 
	//https://claude.ai/share/909371bd-05d5-49ab-bdb5-9b4bb5c8a21e
	if(!isdigit(argv[1][0])){
    	printf("mode must be an integer\n");
    	return 1;
	}
	if(!isdigit(argv[2][0])){
    	printf("min must be an integer\n");
    	return 1;
	}
	if(!isdigit(argv[3][0])){
    	printf("max must be an integer\n");
    	return 1;
	}

	
	//gets the values from the arguments and stores them in variables
	int mode = atoi(argv[1]);
	int min = atoi(argv[2]);
	int max = atoi(argv[3]);

	//checks if min is less than max
	if(max <= min){
    	printf("max %d must be greater than min %d\n", max, min);
    	return 1;
	}

	//normalizes the mode if anything other than 0 to 1 (parallel)
	if(mode != 0){
		mode = 1;
	}	

	//divides the given range into quartiles	
	int q1Start = min;
	int q1End = min + (max - min) * 1/4;
	int q2End = min + (max - min) * 2/4;
	int q3End = min + (max - min) * 3/4;
	int q4End = max;

	printf("Process id: %d\n", getpid());

	if(mode == 0){//serial mode

		int count;
		long long sum;

		//calculates the primes and displays results for each quarile one by one (serial)
		calculatePrime(q1Start, q1End, &count, &sum);
		printQuarterResult(q1Start, q1End, count, sum);

		calculatePrime(q1End, q2End, &count, &sum);
		printQuarterResult(q1End, q2End, count, sum);

		calculatePrime(q2End, q3End, &count, &sum);
		printQuarterResult(q2End, q3End, count, sum);

		calculatePrime(q3End, q4End, &count, &sum);
		printQuarterResult(q3End, q4End, count, sum);
		
	}
	else{//parallel mode
		int starts[] = {q1Start, q1End, q2End, q3End};
		int ends[] = {q1End, q2End, q3End, q4End};

		for(int i = 0; i < 4; i++){
			pid_t pid = fork();//creates child process

			if(pid < 0){//checks if the fork worked 
				perror("fork");
				return 1;
			}
			else if(pid == 0){//child process
				int count;
				long long sum;
				
				//each child will calculate a quartile of of primes and print their result
				calculatePrime(starts[i], ends[i], &count, &sum);
				printQuarterResult(starts[i], ends[i], count, sum);
				
				exit(0);//ensures the child does not call additonal forks 
			}
		}

		for (int i = 0; i < 4; i++){//parent waits for all children
			wait(NULL);
		}
	}

	return 0;
}
