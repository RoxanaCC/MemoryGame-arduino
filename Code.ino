byte sequence[100];	//the random sequence
byte seqLen = 0;	//current lenght used of the sequence
byte inputCount = 0;	//number of pressed buttons
byte expect = 0;	//expected input
byte lastInput = 0;

byte nrPins = 4;
byte Buttonpins[] = {3, 4, 6, 7};
byte Ledpins[] = {8, 9, 11, 12};

bool wait = false;	//check if it's the player's turn
bool resetFlag = false;   
bool press = false;	//check if a button was pressed
bool start = false;	//check if the start button was pressed

void setup() {
	for(byte i = 0; i < nrPins; i++){
		pinMode(Buttonpins[i], INPUT); 
	}
    
	for(byte i = 0; i < nrPins; i++){
		pinMode(Ledpins[i], OUTPUT); 
	} 
	Reset();
}

void flash(byte nr){	//function to make all the leds flash
	for(int j = 0; j < nr; j++){
		for(byte i = 0; i < nrPins; i++){
			digitalWrite(Ledpins[i], HIGH); 
		}
		delay(150);
		for(byte i = 0; i < nrPins; i++){
			digitalWrite(Ledpins[i], LOW); 
		}
		delay(150);
	}
}
  
void Reset(){		//fuction to reset the game and all the variables
	randomSeed(analogRead(A0));
	for(int i = 0; i < 100; i++){
		sequence[i] = random(4);
	}
	seqLen = 0;
	inputCount = 0;
	expect = 0;
	lastInput = 0;
	wait = false;
	resetFlag = false;
	press = false;
	start = false;
}

void playSequence(){		//function to play the current sequence
	for(int i = 0; i < seqLen; i++){
		digitalWrite(Ledpins[sequence[i]], HIGH);
		delay(500);
		digitalWrite(Ledpins[sequence[i]], LOW);
		delay(250);
	} 
}

void Lose(){			//function for when the player looses
	for(byte i = 0; i < 5; i++){
		digitalWrite(Ledpins[0], HIGH); 
		delay(100);
		digitalWrite(Ledpins[0], LOW);
		delay(100);
	}
	flash(1);
	delay(00);
	playSequence();
	delay(500);
	flash(1);
	delay(500);

	Reset();
}

void Win(){			//function for when the player wins
	digitalWrite(Ledpins[2], HIGH); 
	delay(300);
	digitalWrite(Ledpins[2], LOW);
	delay(100);
	for(byte i = 0; i < 5; i++){
	  digitalWrite(Ledpins[0], HIGH); 
	  delay(100);
	  digitalWrite(Ledpins[1], HIGH); 
	  delay(100);
	  digitalWrite(Ledpins[0], LOW);
	  digitalWrite(Ledpins[2], HIGH); 
	  delay(100);
	  digitalWrite(Ledpins[1], LOW);
	  digitalWrite(Ledpins[3], HIGH); 
	  delay(100);
	  digitalWrite(Ledpins[2], LOW);
	  delay(100);
	  digitalWrite(Ledpins[2], HIGH); 
	  delay(100);
	  digitalWrite(Ledpins[3], LOW);
	  digitalWrite(Ledpins[1], HIGH); 
	  delay(100);
	  digitalWrite(Ledpins[2], LOW);
	  digitalWrite(Ledpins[0], HIGH); 
	  delay(100);
	  digitalWrite(Ledpins[1], LOW);
	}
	digitalWrite(Ledpins[0], LOW);
	delay(500);
	Reset();
}

void loop() {
  
	if(digitalRead(13) == HIGH){		//checking if the start button was pressed
		start = true;
	}
	if(start == true){		//game can begin
		if(wait == false){		//showing the sequence
			seqLen++;
			delay(1000);
			playSequence();
			wait = true;
		}
		else{			//player's turn
			if(press == false){		//checking if a wrong button was pressed
				expect = sequence[inputCount];
				for(int i = 0; i < nrPins; i++){
					if(i == expect)                        
						continue;
					if(digitalRead(Buttonpins[i]) == HIGH){
						digitalWrite(Ledpins[i], HIGH);
						resetFlag = true;
						press = true;
						lastInput = i;
					}
				}  
			}
			if(digitalRead(Buttonpins[expect]) == HIGH && press == false){		//checking if the right button was pressed
				digitalWrite(Ledpins[expect], HIGH);
				inputCount++;
				press = true;
				lastInput = expect;
			}
			else{
				if(press == true && digitalRead(Buttonpins[lastInput]) == LOW){		//checking if the player released the button
					digitalWrite(Ledpins[lastInput], LOW);
					press = false;
					delay(20);
					if(resetFlag == true){		//reset the game if the player pressed a wrong button
						Lose();
					}
					else{
						if(inputCount == seqLen){	//the player complited the curent sequence
							wait = false;
							flash(3);
							inputCount = 0;
							if(seqLen == 100){	//the player finished the game
								Win();
							}
						}
					}
				}
			}
		}
	}
}
