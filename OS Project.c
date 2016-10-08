#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>

struct tag_transaction{
	int transaction_number;
	int arrival_time;
	char name[50];
	char session[12];
	char task[8];
	char admin_channel[24];
	int reservation_number;
};

struct tag_reservation{
	int reservation_number;
	char name[50];
	char session[12];
};

struct tag_transaction transaction_day1[25];
struct tag_reservation reservation_day1[25];
int reservation_day1_count = 0;

struct tag_transaction transaction_day2[25];
struct tag_reservation reservation_day2[25];
int reservation_day2_count = 0;

CRITICAL_SECTION cs_day1;
CRITICAL_SECTION cs_day2;

void strip_newline(char str[], int size){
     
   int i;
     
   for(i = 0; i <= size - 1; i++){
         
       if(str[i] == '\n'){
            
           str[i] = '\0';
            
       }
         
   }

}

// returns 1 if reservation number is unique or 0 if reservation number is not unique
int is_reservation_number_unique(int reservation_number, char *day){

	int i;

	if(strcmp(day,"Day 1") == 0){

	   for(i = 0; i <= 24; i++){

	       if(reservation_day1[i].reservation_number == reservation_number){

		       return 0;
		   }
	   }

	   return 1;
	}
	else{
		// Day 2
		for(i = 0; i <= 24; i++){

		    if(reservation_day2[i].reservation_number == reservation_number){

			    return 0;
			}
		}
	    return 1;

	}
}


// return index of reservation or -1 if not found
int get_reservation_index(int reservation_number, char *name, char *session, char *day){

	int i;

	if(strcmp(day,"Day 1") == 0){
	
	    for(i = 0; i <= 24; i++){

		    if(reservation_day1[i].reservation_number == reservation_number && strcmp(reservation_day1[i].name,name) == 0 && strcmp(reservation_day1[i].session,session) == 0){
			    return i;
		    }
		}
	    return -1;
	}
	else{
		// Day 2
		for(i = 0; i <= 24; i++){

		    if(reservation_day2[i].reservation_number == reservation_number && strcmp(reservation_day2[i].name,name) == 0 && strcmp(reservation_day2[i].session,session) == 0){
			    return i;
		     }
		}
	    return -1;
	}
}

// returns 1 if reservation exists or 0 if it does not
int reservation_exists(int reservation_number, char *name, char *session, char *day){

	int i;
	
	if(strcmp(day,"Day 1") == 0){
	
	    for(i = 0; i <= 24; i++){

		    if(reservation_day1[i].reservation_number == reservation_number && strcmp(reservation_day1[i].name,name) == 0 && strcmp(reservation_day1[i].session,session) == 0){
			    return 1;
			}
		}
		return 0;
	}
	else{
		// Day 2
		 for(i = 0; i <= 24; i++){

		    if(reservation_day2[i].reservation_number == reservation_number && strcmp(reservation_day2[i].name,name) == 0 && strcmp(reservation_day2[i].session,session) == 0){
			    return 1;
			}
		}
		return 0;
	}
}

// returns 1 if session is full, 0 if not full, -1 if error
int is_full(char *session, char *day){

	int i;
	int count = 0;

	if(strcmp(day,"Day 1") == 0){

	    if(strcmp(session,"Session 1") == 0){

		    for(i = 0; i <= 24; i++){
			   // check if there is a reservation for session 1
			   if(reservation_day1[i].reservation_number > 0 && reservation_day1[i].name[0] != '\0' && strcmp(reservation_day1[i].session,"Session 1") == 0){
			       count++;
			   }
			}

		    if (count == 10){

			   return 1;
		    }
		    else{

			   return 0;
		    }
		}
	    else if(strcmp(session,"Session 2") == 0){

		    for(i = 0; i <= 24; i++){
			    // check if there is a reservation for session 2
			    if(reservation_day1[i].reservation_number > 0 && reservation_day1[i].name[0] != '\0' && strcmp(reservation_day1[i].session,"Session 2") == 0){
				    count++;
			    }

		    }

		    if (count == 15){

			   return 1;
		    }
		    else{

			    return 0;

		    }

	    }
	}
	else{
		// Day 2
		if(strcmp(session,"Session 1") == 0){

		    for(i = 0; i <= 24; i++){
			   // check if there is a reservation for session 1
			   if(reservation_day2[i].reservation_number > 0 && reservation_day2[i].name[0] != '\0' && strcmp(reservation_day2[i].session,"Session 1") == 0){
			       count++;
			   }
			}

		    if (count == 10){

			   return 1;
		    }
		    else{

			   return 0;
		    }
		}
	    else if(strcmp(session,"Session 2") == 0){

		    for(i = 0; i <= 24; i++){
			    // check if there is a reservation for session 2
			    if(reservation_day2[i].reservation_number > 0 && reservation_day2[i].name[0] != '\0' && strcmp(reservation_day2[i].session,"Session 2") == 0){
				    count++;
			    }

		    }

		    if (count == 15){

			   return 1;
		    }
		    else{

			    return 0;

		    }
		}
	}
}

void fork_transactions(char *day){

	if(strcmp(day,"Day 1") == 0){
	transaction_day1[0].transaction_number=1;
	transaction_day1[0].arrival_time=randr(0,40);
	strcpy(transaction_day1[0].name,"Trans 1");
	strcpy(transaction_day1[0].session,"Session 1");
	strcpy(transaction_day1[0].task,"Make");
	strcpy(transaction_day1[0].admin_channel,"Admin Channel 1");
	transaction_day1[0].reservation_number=NULL;

	transaction_day1[1].transaction_number=2;
	transaction_day1[1].arrival_time=randr(transaction_day1[0].arrival_time,40);
	strcpy(transaction_day1[1].name,"Trans 2");
	strcpy(transaction_day1[1].session,"Session 1");
	strcpy(transaction_day1[1].task,"Make");
	strcpy(transaction_day1[1].admin_channel,"Admin Channel 2");
	transaction_day1[1].reservation_number=NULL;

	transaction_day1[2].transaction_number=3;
	transaction_day1[2].arrival_time=randr(transaction_day1[1].arrival_time,40);
	strcpy(transaction_day1[2].name,"Trans 3");
	strcpy(transaction_day1[2].session,"Session 1");
	strcpy(transaction_day1[2].task,"Make");
	strcpy(transaction_day1[2].admin_channel,"Admin Channel 3");
	transaction_day1[2].reservation_number=NULL;

	transaction_day1[3].transaction_number=4;
	transaction_day1[3].arrival_time=randr(transaction_day1[2].arrival_time,40);
	strcpy(transaction_day1[3].name,"Trans 4");
	strcpy(transaction_day1[3].session,"Session 1");
	strcpy(transaction_day1[3].task,"Make");
	strcpy(transaction_day1[3].admin_channel,"Admin Channel 4");
	transaction_day1[3].reservation_number=NULL;

	transaction_day1[4].transaction_number=5;
	transaction_day1[4].arrival_time=randr(transaction_day1[3].arrival_time,40);
	strcpy(transaction_day1[4].name,"Trans 5");
	strcpy(transaction_day1[4].session,"Session 1");
	strcpy(transaction_day1[4].task,"Make");
	strcpy(transaction_day1[4].admin_channel,"Admin Channel 1");
	transaction_day1[4].reservation_number=NULL;

	transaction_day1[5].transaction_number=6;
	transaction_day1[5].arrival_time=randr(transaction_day1[4].arrival_time,40);
	strcpy(transaction_day1[5].name,"Trans 6");
	strcpy(transaction_day1[5].session,"Session 1");
	strcpy(transaction_day1[5].task,"Make");
	strcpy(transaction_day1[5].admin_channel,"Admin Channel 2");
	transaction_day1[5].reservation_number=NULL;

	transaction_day1[6].transaction_number=7;
	transaction_day1[6].arrival_time=randr(transaction_day1[5].arrival_time,40);
	strcpy(transaction_day1[6].name,"Trans 7");
	strcpy(transaction_day1[6].session,"Session 1");
	strcpy(transaction_day1[6].task,"Make");
	strcpy(transaction_day1[6].admin_channel,"Admin Channel 3");
	transaction_day1[6].reservation_number=NULL;

	transaction_day1[7].transaction_number=8;
	transaction_day1[7].arrival_time=randr(transaction_day1[6].arrival_time,40);
	strcpy(transaction_day1[7].name,"Trans 8");
	strcpy(transaction_day1[7].session,"Session 1");
	strcpy(transaction_day1[7].task,"Make");
	strcpy(transaction_day1[7].admin_channel,"Admin Channel 4");
	transaction_day1[7].reservation_number=NULL;

	transaction_day1[8].transaction_number=9;
	transaction_day1[8].arrival_time=randr(transaction_day1[7].arrival_time,40);
	strcpy(transaction_day1[8].name,"Trans 9");
	strcpy(transaction_day1[8].session,"Session 1");
	strcpy(transaction_day1[8].task,"Make");
	strcpy(transaction_day1[8].admin_channel,"Admin Channel 4");
	transaction_day1[8].reservation_number=NULL;

	transaction_day1[9].transaction_number=10;
	transaction_day1[9].arrival_time=randr(transaction_day1[8].arrival_time,40);
	strcpy(transaction_day1[9].name,"Trans 10");
	strcpy(transaction_day1[9].session,"Session 1");
	strcpy(transaction_day1[9].task,"Make");
	strcpy(transaction_day1[9].admin_channel,"Admin Channel 4");
	transaction_day1[9].reservation_number=NULL;

	transaction_day1[10].transaction_number=11;
	transaction_day1[10].arrival_time=randr(transaction_day1[9].arrival_time,40);
	strcpy(transaction_day1[10].name,"Trans 11");
	strcpy(transaction_day1[10].session,"Session 2");
	strcpy(transaction_day1[10].task,"Make");
	strcpy(transaction_day1[10].admin_channel,"Admin Channel 1");
	transaction_day1[10].reservation_number=NULL;

	transaction_day1[11].transaction_number=12;
	transaction_day1[11].arrival_time=randr(transaction_day1[10].arrival_time,40);
	strcpy(transaction_day1[11].name,"Trans 12");
	strcpy(transaction_day1[11].session,"Session 2");
	strcpy(transaction_day1[11].task,"Make");
	strcpy(transaction_day1[11].admin_channel,"Admin Channel 2");
	transaction_day1[11].reservation_number=NULL;

	transaction_day1[12].transaction_number=13;
	transaction_day1[12].arrival_time=randr(transaction_day1[11].arrival_time,40);
	strcpy(transaction_day1[12].name,"Trans 13");
	strcpy(transaction_day1[12].session,"Session 2");
	strcpy(transaction_day1[12].task,"Make");
	strcpy(transaction_day1[12].admin_channel,"Admin Channel 3");
	transaction_day1[12].reservation_number=NULL;

	transaction_day1[13].transaction_number=14;
	transaction_day1[13].arrival_time=randr(transaction_day1[12].arrival_time,40);
	strcpy(transaction_day1[13].name,"Trans 14");
	strcpy(transaction_day1[13].session,"Session 2");
	strcpy(transaction_day1[13].task,"Make");
	strcpy(transaction_day1[13].admin_channel,"Admin Channel 4");
	transaction_day1[13].reservation_number=NULL;

	transaction_day1[14].transaction_number=15;
	transaction_day1[14].arrival_time=randr(transaction_day1[13].arrival_time,40);
	strcpy(transaction_day1[14].name,"Trans 15");
	strcpy(transaction_day1[14].session,"Session 2");
	strcpy(transaction_day1[14].task,"Make");
	strcpy(transaction_day1[14].admin_channel,"Admin Channel 1");
	transaction_day1[14].reservation_number=NULL;

	transaction_day1[15].transaction_number=16;
	transaction_day1[15].arrival_time=randr(transaction_day1[14].arrival_time,40);
	strcpy(transaction_day1[15].name,"Trans 16");
	strcpy(transaction_day1[15].session,"Session 2");
	strcpy(transaction_day1[15].task,"Make");
	strcpy(transaction_day1[15].admin_channel,"Admin Channel 3");
	transaction_day1[15].reservation_number=NULL;

	transaction_day1[16].transaction_number=17;
	transaction_day1[16].arrival_time=randr(transaction_day1[15].arrival_time,40);
	strcpy(transaction_day1[16].name,"Trans 17");
	strcpy(transaction_day1[16].session,"Session 2");
	strcpy(transaction_day1[16].task,"Make");
	strcpy(transaction_day1[16].admin_channel,"Admin Channel 2");
	transaction_day1[16].reservation_number=NULL;

	transaction_day1[17].transaction_number=18;
	transaction_day1[17].arrival_time=randr(transaction_day1[16].arrival_time,40);
	strcpy(transaction_day1[17].name,"Trans 18");
	strcpy(transaction_day1[17].session,"Session 2");
	strcpy(transaction_day1[17].task,"Make");
	strcpy(transaction_day1[17].admin_channel,"Admin Channel 4");
	transaction_day1[17].reservation_number=NULL;

	transaction_day1[18].transaction_number=19;
	transaction_day1[18].arrival_time=randr(transaction_day1[17].arrival_time,40);
	strcpy(transaction_day1[18].name,"Trans 19");
	strcpy(transaction_day1[18].session,"Session 2");
	strcpy(transaction_day1[18].task,"Make");
	strcpy(transaction_day1[18].admin_channel,"Admin Channel 1");
	transaction_day1[18].reservation_number=NULL;

	transaction_day1[19].transaction_number=20;
	transaction_day1[19].arrival_time=randr(transaction_day1[18].arrival_time,40);
	strcpy(transaction_day1[19].name,"Trans 20");
	strcpy(transaction_day1[19].session,"Session 2");
	strcpy(transaction_day1[19].task,"Make");
	strcpy(transaction_day1[19].admin_channel,"Admin Channel 1");
	transaction_day1[19].reservation_number=NULL;

	transaction_day1[20].transaction_number=21;
	transaction_day1[20].arrival_time=randr(transaction_day1[19].arrival_time,40);
	strcpy(transaction_day1[20].name,"Trans 21");
	strcpy(transaction_day1[20].session,"Session 2");
	strcpy(transaction_day1[20].task,"Make");
	strcpy(transaction_day1[20].admin_channel,"Admin Channel 3");
	transaction_day1[20].reservation_number=NULL;

	transaction_day1[21].transaction_number=22;
	transaction_day1[21].arrival_time=randr(transaction_day1[20].arrival_time,40);
	strcpy(transaction_day1[21].name,"Trans 22");
	strcpy(transaction_day1[21].session,"Session 2");
	strcpy(transaction_day1[21].task,"Make");
	strcpy(transaction_day1[21].admin_channel,"Admin Channel 4");
	transaction_day1[21].reservation_number=NULL;

	transaction_day1[22].transaction_number=23;
	transaction_day1[22].arrival_time=randr(transaction_day1[21].arrival_time,40);
	strcpy(transaction_day1[22].name,"Trans 23");
	strcpy(transaction_day1[22].session,"Session 2");
	strcpy(transaction_day1[22].task,"Make");
	strcpy(transaction_day1[22].admin_channel,"Admin Channel 2");
	transaction_day1[22].reservation_number=NULL;


	transaction_day1[23].transaction_number=24;
	transaction_day1[23].arrival_time=randr(transaction_day1[22].arrival_time,40);
	strcpy(transaction_day1[23].name,"Trans 24");
	strcpy(transaction_day1[23].session,"Session 2");
	strcpy(transaction_day1[23].task,"Make");
	strcpy(transaction_day1[23].admin_channel,"Admin Channel 4");
	transaction_day1[23].reservation_number=NULL;

	transaction_day1[24].transaction_number=25;
	transaction_day1[24].arrival_time=randr(transaction_day1[23].arrival_time,40);
	strcpy(transaction_day1[24].name,"Trans 25");
	strcpy(transaction_day1[24].session,"Session 2");
	strcpy(transaction_day1[24].task,"Make");
	strcpy(transaction_day1[24].admin_channel,"Admin Channel 2");
	transaction_day1[24].reservation_number=NULL;
   

	}

	else
	{

		transaction_day2[0].transaction_number=1;
	transaction_day2[0].arrival_time=randr(0,40);
	strcpy(transaction_day2[0].name,"Trans 1");
	strcpy(transaction_day2[0].session,"Session 1");
	strcpy(transaction_day2[0].task,"Make");
	strcpy(transaction_day2[0].admin_channel,"Admin Channel 1");
	transaction_day2[0].reservation_number=NULL;

	transaction_day2[1].transaction_number=2;
	transaction_day2[1].arrival_time=randr(0,40);
	strcpy(transaction_day2[1].name,"Trans 2");
	strcpy(transaction_day2[1].session,"Session 1");
	strcpy(transaction_day2[1].task,"Make");
	strcpy(transaction_day2[1].admin_channel,"Admin Channel 2");
	transaction_day2[1].reservation_number=NULL;

	transaction_day2[2].transaction_number=3;
	transaction_day2[2].arrival_time=randr(0,40);
	strcpy(transaction_day2[2].name,"Trans 3");
	strcpy(transaction_day2[2].session,"Session 1");
	strcpy(transaction_day2[2].task,"Make");
	strcpy(transaction_day2[2].admin_channel,"Admin Channel 3");
	transaction_day2[2].reservation_number=NULL;

	transaction_day2[3].transaction_number=4;
	transaction_day2[3].arrival_time=randr(0,40);
	strcpy(transaction_day2[3].name,"Trans 4");
	strcpy(transaction_day2[3].session,"Session 1");
	strcpy(transaction_day2[3].task,"Make");
	strcpy(transaction_day2[3].admin_channel,"Admin Channel 4");
	transaction_day2[3].reservation_number=NULL;

	transaction_day2[4].transaction_number=5;
	transaction_day2[4].arrival_time=randr(0,40);
	strcpy(transaction_day2[4].name,"Trans 5");
	strcpy(transaction_day2[4].session,"Session 1");
	strcpy(transaction_day2[4].task,"Make");
	strcpy(transaction_day2[4].admin_channel,"Admin Channel 1");
	transaction_day2[4].reservation_number=NULL;

	transaction_day2[5].transaction_number=6;
	transaction_day2[5].arrival_time=randr(0,40);
	strcpy(transaction_day2[5].name,"Trans 6");
	strcpy(transaction_day2[5].session,"Session 1");
	strcpy(transaction_day2[5].task,"Make");
	strcpy(transaction_day2[5].admin_channel,"Admin Channel 2");
	transaction_day2[5].reservation_number=NULL;

	transaction_day2[6].transaction_number=7;
	transaction_day2[6].arrival_time=randr(0,40);
	strcpy(transaction_day2[6].name,"Trans 7");
	strcpy(transaction_day2[6].session,"Session 1");
	strcpy(transaction_day2[6].task,"Make");
	strcpy(transaction_day2[6].admin_channel,"Admin Channel 3");
	transaction_day2[6].reservation_number=NULL;

	transaction_day2[7].transaction_number=8;
	transaction_day2[7].arrival_time=randr(0,40);
	strcpy(transaction_day2[7].name,"Trans 8");
	strcpy(transaction_day2[7].session,"Session 1");
	strcpy(transaction_day2[7].task,"Make");
	strcpy(transaction_day2[7].admin_channel,"Admin Channel 4");
	transaction_day2[7].reservation_number=NULL;

	transaction_day2[8].transaction_number=9;
	transaction_day2[8].arrival_time=randr(0,40);
	strcpy(transaction_day2[8].name,"Trans 9");
	strcpy(transaction_day2[8].session,"Session 1");
	strcpy(transaction_day2[8].task,"Make");
	strcpy(transaction_day2[8].admin_channel,"Admin Channel 4");
	transaction_day2[8].reservation_number=NULL;

	transaction_day2[9].transaction_number=10;
	transaction_day2[9].arrival_time=randr(0,40);
	strcpy(transaction_day2[9].name,"Trans 10");
	strcpy(transaction_day2[9].session,"Session 1");
	strcpy(transaction_day2[9].task,"Make");
	strcpy(transaction_day2[9].admin_channel,"Admin Channel 4");
	transaction_day2[9].reservation_number=NULL;

	transaction_day2[10].transaction_number=11;
	transaction_day2[10].arrival_time=randr(0,40);
	strcpy(transaction_day2[10].name,"Trans 11");
	strcpy(transaction_day2[10].session,"Session 2");
	strcpy(transaction_day2[10].task,"Make");
	strcpy(transaction_day2[10].admin_channel,"Admin Channel 1");
	transaction_day2[10].reservation_number=NULL;

	transaction_day2[11].transaction_number=12;
	transaction_day2[11].arrival_time=randr(0,40);
	strcpy(transaction_day2[11].name,"Trans 12");
	strcpy(transaction_day2[11].session,"Session 2");
	strcpy(transaction_day2[11].task,"Make");
	strcpy(transaction_day2[11].admin_channel,"Admin Channel 2");
	transaction_day2[11].reservation_number=NULL;

	transaction_day2[12].transaction_number=13;
	transaction_day2[12].arrival_time=randr(0,40);
	strcpy(transaction_day2[12].name,"Trans 13");
	strcpy(transaction_day2[12].session,"Session 2");
	strcpy(transaction_day2[12].task,"Make");
	strcpy(transaction_day2[12].admin_channel,"Admin Channel 3");
	transaction_day2[12].reservation_number=NULL;

	transaction_day2[13].transaction_number=14;
	transaction_day2[13].arrival_time=randr(0,40);
	strcpy(transaction_day2[13].name,"Trans 14");
	strcpy(transaction_day2[13].session,"Session 2");
	strcpy(transaction_day2[13].task,"Make");
	strcpy(transaction_day2[13].admin_channel,"Admin Channel 4");
	transaction_day2[13].reservation_number=NULL;

	transaction_day2[14].transaction_number=15;
	transaction_day2[14].arrival_time=randr(0,40);
	strcpy(transaction_day2[14].name,"Trans 15");
	strcpy(transaction_day2[14].session,"Session 2");
	strcpy(transaction_day2[14].task,"Make");
	strcpy(transaction_day2[14].admin_channel,"Admin Channel 1");
	transaction_day2[14].reservation_number=NULL;

	transaction_day2[15].transaction_number=16;
	transaction_day2[15].arrival_time=randr(0,40);
	strcpy(transaction_day2[15].name,"Trans 16");
	strcpy(transaction_day2[15].session,"Session 2");
	strcpy(transaction_day2[15].task,"Make");
	strcpy(transaction_day2[15].admin_channel,"Admin Channel 3");
	transaction_day2[15].reservation_number=NULL;

	transaction_day2[16].transaction_number=17;
	transaction_day2[16].arrival_time=randr(0,40);
	strcpy(transaction_day2[16].name,"Trans 17");
	strcpy(transaction_day2[16].session,"Session 2");
	strcpy(transaction_day2[16].task,"Make");
	strcpy(transaction_day2[16].admin_channel,"Admin Channel 2");
	transaction_day2[16].reservation_number=NULL;

	transaction_day2[17].transaction_number=18;
	transaction_day2[17].arrival_time=randr(0,40);
	strcpy(transaction_day2[17].name,"Trans 18");
	strcpy(transaction_day2[17].session,"Session 2");
	strcpy(transaction_day2[17].task,"Make");
	strcpy(transaction_day2[17].admin_channel,"Admin Channel 4");
	transaction_day2[17].reservation_number=NULL;

	transaction_day2[18].transaction_number=19;
	transaction_day2[18].arrival_time=randr(0,40);
	strcpy(transaction_day2[18].name,"Trans 19");
	strcpy(transaction_day2[18].session,"Session 2");
	strcpy(transaction_day2[18].task,"Make");
	strcpy(transaction_day2[18].admin_channel,"Admin Channel 1");
	transaction_day2[18].reservation_number=NULL;

	transaction_day2[19].transaction_number=20;
	transaction_day2[19].arrival_time=randr(0,40);
	strcpy(transaction_day2[19].name,"Trans 20");
	strcpy(transaction_day2[19].session,"Session 2");
	strcpy(transaction_day2[19].task,"Make");
	strcpy(transaction_day2[19].admin_channel,"Admin Channel 1");
	transaction_day2[19].reservation_number=NULL;

	transaction_day2[20].transaction_number=21;
	transaction_day2[20].arrival_time=randr(0,40);
	strcpy(transaction_day2[20].name,"Trans 21");
	strcpy(transaction_day2[20].session,"Session 2");
	strcpy(transaction_day2[20].task,"Make");
	strcpy(transaction_day2[20].admin_channel,"Admin Channel 3");
	transaction_day2[20].reservation_number=NULL;

	transaction_day2[21].transaction_number=22;
	transaction_day2[21].arrival_time=randr(0,40);
	strcpy(transaction_day2[21].name,"Trans 22");
	strcpy(transaction_day2[21].session,"Session 2");
	strcpy(transaction_day2[21].task,"Make");
	strcpy(transaction_day2[21].admin_channel,"Admin Channel 4");
	transaction_day2[21].reservation_number=NULL;

	transaction_day2[22].transaction_number=23;
	transaction_day2[22].arrival_time=randr(0,40);
	strcpy(transaction_day2[22].name,"Trans 23");
	strcpy(transaction_day2[22].session,"Session 2");
	strcpy(transaction_day2[22].task,"Make");
	strcpy(transaction_day2[22].admin_channel,"Admin Channel 2");
	transaction_day2[22].reservation_number=NULL;


	transaction_day2[23].transaction_number=24;
	transaction_day2[23].arrival_time=randr(0,40);
	strcpy(transaction_day2[23].name,"Trans 24");
	strcpy(transaction_day2[23].session,"Session 2");
	strcpy(transaction_day2[23].task,"Make");
	strcpy(transaction_day2[23].admin_channel,"Admin Channel 4");
	transaction_day2[23].reservation_number=NULL;

	transaction_day2[24].transaction_number=25;
	transaction_day2[24].arrival_time=randr(0,40);
	strcpy(transaction_day2[24].name,"Trans 25");
	strcpy(transaction_day2[24].session,"Session 2");
	strcpy(transaction_day2[24].task,"Make");
	strcpy(transaction_day2[24].admin_channel,"Admin Channel 2");
	transaction_day2[24].reservation_number=NULL;
   


	}
}

 unsigned int randr(unsigned int min, unsigned int max)
{
       double scaled = (double)rand()/RAND_MAX;

       return (max - min +1)*scaled + min;
}



// First Come, First Served (FCFS)
void schedule_transactions(char *day){
	int i;
	int j;

	int is_swap;
	
	int temp_transaction_number;
	int temp_arrival_time;
	char temp_name[50];
	char temp_session[12];
	char temp_task[8];
	char temp_admin_channel[24];
	int temp_reservation_number;

	if(strcmp(day,"Day 1") == 0){

	    for(i = 0; i <= 24; i++){

			is_swap = 0;

			for(j = 0; j <= 23; j++){

				if(transaction_day1[j].arrival_time > transaction_day1[j + 1].arrival_time){
					
					temp_transaction_number = transaction_day1[j].transaction_number;
					temp_arrival_time = transaction_day1[j].arrival_time;
					strcpy(temp_name,transaction_day1[j].name);
					strcpy(temp_session,transaction_day1[j].session);
					strcpy(temp_task,transaction_day1[j].task);
					strcpy(temp_admin_channel,transaction_day1[j].admin_channel);
				    temp_reservation_number = transaction_day1[j].reservation_number;

					transaction_day1[j].transaction_number = transaction_day1[j + 1].transaction_number;
					transaction_day1[j].arrival_time = transaction_day1[j + 1].arrival_time;
					strcpy(transaction_day1[j].name,transaction_day1[j + 1].name);
					strcpy(transaction_day1[j].session,transaction_day1[j + 1].session);
					strcpy(transaction_day1[j].task,transaction_day1[j + 1].task);
					strcpy(transaction_day1[j].admin_channel,transaction_day1[j + 1].admin_channel);
					transaction_day1[j].reservation_number = transaction_day1[j + 1].reservation_number;

					transaction_day1[j + 1].transaction_number = temp_transaction_number;
					transaction_day1[j + 1].arrival_time = temp_arrival_time;
					strcpy(transaction_day1[j + 1].name, temp_name);
					strcpy(transaction_day1[j + 1].session, temp_session);
					strcpy(transaction_day1[j + 1].task,temp_task);
					strcpy(transaction_day1[j + 1].admin_channel,temp_admin_channel);
					transaction_day1[j + 1].reservation_number = temp_reservation_number;

					is_swap = 1;

				}

			}

			if(is_swap == 0){

				break;
			}
		}

	}
	else{
		// Day 2
		for(i = 0; i <= 24; i++){

			is_swap = 0;

			for(j = 0; j <= 23; j++){

				if(transaction_day2[j].arrival_time > transaction_day2[j + 1].arrival_time){
					
					temp_transaction_number = transaction_day2[j].transaction_number;
					temp_arrival_time = transaction_day2[j].arrival_time;
					strcpy(temp_name,transaction_day2[j].name);
					strcpy(temp_session,transaction_day2[j].session);
					strcpy(temp_task,transaction_day2[j].task);
					strcpy(temp_admin_channel,transaction_day2[j].admin_channel);
				    temp_reservation_number = transaction_day2[j].reservation_number;

					transaction_day2[j].transaction_number = transaction_day2[j + 1].transaction_number;
					transaction_day2[j].arrival_time = transaction_day2[j + 1].arrival_time;
					strcpy(transaction_day2[j].name,transaction_day2[j + 1].name);
					strcpy(transaction_day2[j].session,transaction_day2[j + 1].session);
					strcpy(transaction_day2[j].task,transaction_day2[j + 1].task);
					strcpy(transaction_day2[j].admin_channel,transaction_day2[j + 1].admin_channel);
					transaction_day2[j].reservation_number = transaction_day2[j + 1].reservation_number;

					transaction_day2[j + 1].transaction_number = temp_transaction_number;
					transaction_day2[j + 1].arrival_time = temp_arrival_time;
					strcpy(transaction_day2[j + 1].name, temp_name);
					strcpy(transaction_day2[j + 1].session, temp_session);
					strcpy(transaction_day2[j + 1].task,temp_task);
					strcpy(transaction_day2[j + 1].admin_channel,temp_admin_channel);
					transaction_day2[j + 1].reservation_number = temp_reservation_number;

					is_swap = 1;

				}

			}

			if(is_swap == 0){

				break;
			}
		}

    }

}


struct PARAMS
{
    char *thread_admin_channel;
};

DWORD WINAPI receive_request_day1(void* parameter)
{
    struct PARAMS* params = (struct PARAMS*)parameter;
	int i;
	char session_name[12];
	int reservation_index;
	int random_reservation_number;

	for(i = 0; i <= 24; i++){
		// if request comes in that matches associated admin channel thread function
		if(strcmp(transaction_day1[i].admin_channel,params->thread_admin_channel) == 0){
			
			EnterCriticalSection(&cs_day1);

			 if(strcmp(transaction_day1[i].task,"Make") == 0){
				 // if particular session is not full
				 if(is_full(transaction_day1[i].session,"Day 1") != 1){

			         // make reservation

					 random_reservation_number = rand() % 1000 + 1;
					 while(is_reservation_number_unique(random_reservation_number,"Day 1") == 0){
						  random_reservation_number = rand() % 1000 + 1;
				     }

			         reservation_day1[reservation_day1_count].reservation_number = random_reservation_number;
			         strcpy(reservation_day1[reservation_day1_count].name,transaction_day1[i].name);
			         strcpy(reservation_day1[reservation_day1_count].session,transaction_day1[i].session);

			         //printf("Day 1, %s made reservation: Res #: %d, Name: %s, Session: %s\n\n",params->thread_admin_channel,reservation_day1[reservation_day1_count].reservation_number, reservation_day1[reservation_day1_count].name, reservation_day1[reservation_day1_count].session);

		             reservation_day1_count++;

				 }
				 else{
			         printf("[-] An error occured on Day 1, %s while trying to make the following reservation:\nName: %s, Session: %s\n\n%s is full.\n\n",params->thread_admin_channel,transaction_day1[i].name, transaction_day1[i].session, transaction_day1[i].session); 
				 }
			 }
			 else if(strcmp(transaction_day1[i].task,"Modify") == 0){

			    // check if reservation to be modified exists
				if(reservation_exists(transaction_day1[i].reservation_number,transaction_day1[i].name,transaction_day1[i].session,"Day 1")){
                   
					// get session of reservation to be modified
					strcpy(session_name,transaction_day1[i].session);

					// if "Session 1", cancel and move reservation to "Session 2"
					if(strcmp(session_name,"Session 1") == 0){
					   
						// if Session 2 is not full
						if(is_full("Session 2","Day 1") != 1){
						  
							// modify reservation
							reservation_index = get_reservation_index(transaction_day1[i].reservation_number,transaction_day1[i].name,transaction_day1[i].session,"Day 1");
							strcpy(reservation_day1[reservation_index].session,"Session 2");

						}
						else{

							printf("[-] An error occured on Day 1, %s while trying to modify the following reservation:\nRes #: %d, Name: %s, Session: %s\n\nSession 2 is full.\n\n",params->thread_admin_channel,transaction_day1[i].reservation_number, transaction_day1[i].name, transaction_day1[i].session); 

						}

					}
					// if "Session 2", cancel and move reservation to "Session 1"
					else if(strcmp(session_name,"Session 2") == 0){
					   
						// if Session 1 is not full
						if(is_full("Session 1","Day 1") != 1){

							// modify reservation
							reservation_index = get_reservation_index(transaction_day1[i].reservation_number,transaction_day1[i].name,transaction_day1[i].session,"Day 1");
							strcpy(reservation_day1[reservation_index].session,"Session 1");

						}
						else{

							printf("[-] An error occured on Day 1, %s while trying to modify the following reservation:\nRes #: %d, Name: %s, Session: %s\n\nSession 1 is full.\n\n",params->thread_admin_channel,transaction_day1[i].reservation_number, transaction_day1[i].name, transaction_day1[i].session); 

						}

					}

				}
				else{
					printf("[-] An error occured on Day 1, %s while trying to modify the following reservation:\nRes #: %d, Name: %s, Session: %s\n\nReservation does not exist.\n\n",params->thread_admin_channel,transaction_day1[i].reservation_number, transaction_day1[i].name, transaction_day1[i].session); 

				}
	
			 }

			 LeaveCriticalSection(&cs_day1);
			
		}

	}

    return 0;
}

DWORD WINAPI receive_request_day2(void* parameter)
{
    struct PARAMS* params = (struct PARAMS*)parameter;
	int i;
	char session_name[12];
	int reservation_index;
	int random_reservation_number;

	for(i = 0; i <= 24; i++){
		// if request comes in that matches associated admin channel thread function
		if(strcmp(transaction_day2[i].admin_channel,params->thread_admin_channel) == 0){
			
			EnterCriticalSection(&cs_day2);

			 if(strcmp(transaction_day2[i].task,"Make") == 0){
				 // if particular session is not full
				 if(is_full(transaction_day2[i].session,"Day 2") != 1){

			         // make reservation

					 random_reservation_number = rand() % 2000 + 1001;
					 while(is_reservation_number_unique(random_reservation_number,"Day 2") == 0){
						  random_reservation_number = rand() % 2000 + 1001;
				     }

			         reservation_day2[reservation_day2_count].reservation_number = random_reservation_number;
			         strcpy(reservation_day2[reservation_day2_count].name,transaction_day2[i].name);
			         strcpy(reservation_day2[reservation_day2_count].session,transaction_day2[i].session);

			         //printf("Day 2, %s made reservation: Res #: %d, Name: %s, Session: %s\n\n",params->thread_admin_channel,reservation_day2[reservation_day2_count].reservation_number, reservation_day2[reservation_day2_count].name, reservation_day2[reservation_day2_count].session);

		             reservation_day2_count++;

				 }
				 else{
			         printf("[-] An error occured on Day 2, %s while trying to make the following reservation:\nName: %s, Session: %s\n\n%s is full.\n\n",params->thread_admin_channel,transaction_day2[i].name, transaction_day2[i].session, transaction_day2[i].session); 
				 }
			 }
			 else if(strcmp(transaction_day2[i].task,"Modify") == 0){

			    // check if reservation to be modified exists
				if(reservation_exists(transaction_day2[i].reservation_number,transaction_day2[i].name,transaction_day2[i].session,"Day 2")){
                   
					// get session of reservation to be modified
					strcpy(session_name,transaction_day2[i].session);

					// if "Session 1", cancel and move reservation to "Session 2"
					if(strcmp(session_name,"Session 1") == 0){
					   
						// if Session 2 is not full
						if(is_full("Session 2","Day 2") != 1){
						  
							// modify reservation
							reservation_index = get_reservation_index(transaction_day2[i].reservation_number,transaction_day2[i].name,transaction_day2[i].session,"Day 2");
							strcpy(reservation_day2[reservation_index].session,"Session 2");

						}
						else{

							printf("[-] An error occured on Day 2, %s while trying to modify the following reservation:\nRes #: %d, Name: %s, Session: %s\n\nSession 2 is full.\n\n",params->thread_admin_channel,transaction_day2[i].reservation_number, transaction_day2[i].name, transaction_day2[i].session); 

						}

					}
					// if "Session 2", cancel and move reservation to "Session 1"
					else if(strcmp(session_name,"Session 2") == 0){
					   
						// if Session 1 is not full
						if(is_full("Session 1","Day 2") != 1){

							// modify reservation
							reservation_index = get_reservation_index(transaction_day2[i].reservation_number,transaction_day2[i].name,transaction_day2[i].session,"Day 2");
							strcpy(reservation_day2[reservation_index].session,"Session 1");

						}
						else{

							printf("[-] An error occured on Day 2, %s while trying to modify the following reservation:\nRes #: %d, Name: %s, Session: %s\n\nSession 1 is full.\n\n",params->thread_admin_channel,transaction_day2[i].reservation_number, transaction_day2[i].name, transaction_day2[i].session); 

						}

					}

				}
				else{
					printf("[-] An error occured on Day 2, %s while trying to modify the following reservation:\nRes #: %d, Name: %s, Session: %s\n\nReservation does not exist.\n\n",params->thread_admin_channel,transaction_day2[i].reservation_number, transaction_day2[i].name, transaction_day2[i].session); 

				}
	
			 }

			 LeaveCriticalSection(&cs_day2);
			
		}

	}

    return 0;
}

void save_reservations(char *day){

	FILE *fp;


	int i;

	if(strcmp(day,"Day 1") == 0){

		fp = fopen("reservations_day1.txt","w");
		if(fp == NULL){
			printf("[-] Unable to open reservations/reservations_day1.txt\n");
			return;
		}

		for(i = 0; i <= 24; i++){
			 if(reservation_day1[i].reservation_number > 0 && reservation_day1[i].name[0] != '\0' && reservation_day1[i].session[0] != '\0'){
			      
			     fprintf(fp,"%d,%s,%s\n",reservation_day1[i].reservation_number,reservation_day1[i].name,reservation_day1[i].session);
			 }
		}

		fclose(fp);
	}
	else{
		// Day 2
		fp = fopen("reservations_day2.txt","w");
		if(fp == NULL){
			printf("[-] Unable to open reservations/reservations_day2.txt\n");
			return;
		}

		for(i = 0; i <= 24; i++){
			if(reservation_day2[i].reservation_number > 0 && reservation_day2[i].name[0] != '\0' && reservation_day2[i].session[0] != '\0'){
			      
			     fprintf(fp,"%d,%s,%s\n",reservation_day2[i].reservation_number,reservation_day2[i].name,reservation_day2[i].session);
			 }
		}

		fclose(fp);
		    
	}

}

void display_report(char *day){

	int i;

	if(strcmp(day,"Day 1") == 0){
		printf("All Transactions on Day 1\n");
		for(i = 0; i <= 2; i++){
			printf("Transaction #: %d\n",transaction_day1[i].transaction_number);
			printf("Arrival Time: %d\n",transaction_day1[i].arrival_time);
			printf("Name: %s\n",transaction_day1[i].name);
			printf("Session: %s\n",transaction_day1[i].task);
			printf("Admin channel: %s\n",transaction_day1[i].admin_channel);
			if(strcmp(transaction_day1[i].task,"Modify") == 0){
				printf("Reservation #: %d\n",transaction_day1[i].reservation_number);

			}
			printf("\n");
		}
		printf("\n");
		printf("All Participants on Day 1\n");
		for(i = 0; i <= 24; i++){
			if(reservation_day1[i].reservation_number > 0 && reservation_day1[i].name[0] != '\0' && reservation_day1[i].session[0] != '\0'){
				printf("Reservation #: %d\n",reservation_day1[i].reservation_number);
				printf("Name: %s\n",reservation_day1[i].name);
				printf("Session: %s\n\n",reservation_day1[i].session);
			     
			 }
		}
	}
	else{
		// Day 2
		printf("All Transactions on Day 2\n");
		for(i = 0; i <= 24; i++){
			printf("Transaction #: %d\n",transaction_day2[i].transaction_number);
			printf("Arrival Time: %d\n",transaction_day2[i].arrival_time);
			printf("Name: %s\n",transaction_day2[i].name);
			printf("Session: %s\n",transaction_day2[i].task);
			printf("Admin channel: %s\n",transaction_day2[i].admin_channel);
			if(strcmp(transaction_day1[i].task,"Modify") == 0){
				printf("Reservation #: %d\n",transaction_day2[i].reservation_number);

			}
			printf("\n");
		}
		printf("\n");
		printf("All Participants on Day 2\n");
		for(i = 0; i <= 24; i++){
			if(reservation_day1[i].reservation_number > 0 && reservation_day1[i].name[0] != '\0' && reservation_day1[i].session[0] != '\0'){
				printf("Reservation #: %d\n",reservation_day2[i].reservation_number);
				printf("Name: %s\n",reservation_day2[i].name);
				printf("Session: %s\n\n",reservation_day2[i].session);
			     
			 }
		}





	}
}


int main(){

	char admin_channel_1_name[] = "Admin Channel 1";
    struct PARAMS admin_channel_1_params;

	char admin_channel_2_name[] = "Admin Channel 2";
    struct PARAMS admin_channel_2_params;

	char admin_channel_3_name[] = "Admin Channel 3";
    struct PARAMS admin_channel_3_params;

	char admin_channel_4_name[] = "Admin Channel 4";
    struct PARAMS admin_channel_4_params;

	HANDLE thread_handle_admin_channel_1;
	HANDLE thread_handle_admin_channel_2;
	HANDLE thread_handle_admin_channel_3;
	HANDLE thread_handle_admin_channel_4;

	int x;

	fork_transactions("Day 1");

	// schedule transactions by arrival time - first come, first served basis
	schedule_transactions("Day 1");

	admin_channel_1_params.thread_admin_channel = admin_channel_1_name;
	admin_channel_2_params.thread_admin_channel = admin_channel_2_name;
    admin_channel_3_params.thread_admin_channel = admin_channel_3_name;
    admin_channel_4_params.thread_admin_channel = admin_channel_4_name;

    srand(time(NULL));

	InitializeCriticalSection(&cs_day1);

	// create a thread for each admin channel that will make reservations for day 1
	thread_handle_admin_channel_1 = CreateThread(NULL, 0, receive_request_day1, &admin_channel_1_params, 0, NULL);
	thread_handle_admin_channel_2 = CreateThread(NULL, 0, receive_request_day1, &admin_channel_2_params, 0, NULL);
	thread_handle_admin_channel_3 = CreateThread(NULL, 0, receive_request_day1, &admin_channel_3_params, 0, NULL);
	thread_handle_admin_channel_4 = CreateThread(NULL, 0, receive_request_day1, &admin_channel_4_params, 0, NULL);


    WaitForSingleObject(thread_handle_admin_channel_1, INFINITE);
	WaitForSingleObject(thread_handle_admin_channel_2, INFINITE);
	WaitForSingleObject(thread_handle_admin_channel_3, INFINITE);
    WaitForSingleObject(thread_handle_admin_channel_4, INFINITE);

	DeleteCriticalSection(&cs_day1);



	fork_transactions("Day 2");

	// schedule transactions by arrival time - first come, first served basis
	schedule_transactions("Day 2");

	InitializeCriticalSection(&cs_day2);

	// create a thread for each admin channel that will make reservations for day 2
	thread_handle_admin_channel_1 = CreateThread(NULL, 0, receive_request_day2, &admin_channel_1_params, 0, NULL);
	thread_handle_admin_channel_2 = CreateThread(NULL, 0, receive_request_day2, &admin_channel_2_params, 0, NULL);
	thread_handle_admin_channel_3 = CreateThread(NULL, 0, receive_request_day2, &admin_channel_3_params, 0, NULL);
	thread_handle_admin_channel_4 = CreateThread(NULL, 0, receive_request_day2, &admin_channel_4_params, 0, NULL);

	WaitForSingleObject(thread_handle_admin_channel_1, INFINITE);
	WaitForSingleObject(thread_handle_admin_channel_2, INFINITE);
	WaitForSingleObject(thread_handle_admin_channel_3, INFINITE);
    WaitForSingleObject(thread_handle_admin_channel_4, INFINITE);

	DeleteCriticalSection(&cs_day2);

	save_reservations("Day 1"); 
	save_reservations("Day 2");

	system("pause");  
	printf("************************ SYSTEM REPORT *********************************\n");
	display_report("Day 1");

	system("pause");
	display_report("Day 2");

	
	printf("[+] Simulation ended successfully\n");
	system("pause");

	return 0;

}