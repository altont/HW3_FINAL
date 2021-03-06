// Ta_Alton_ME493_ProjectBeta.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <vector>
#include <time.h>
#include <random>
#include <array>
#include <string.h>
#include <string>
#include <sstream>
#include <assert.h>
#include <algorithm>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <limits>
#include <ostream>
#include <iterator>
#include <math.h>
#include <iomanip>

using namespace std;

#define ATRAND (double)rand()/RAND_MAX



/////////////////////////////////////////////////
/////////////// AGENT STUFF /////////////////////
/////////////////////////////////////////////////



class agent {
public:
	int ax;
	int ay;
	int skip_move;
	int direction;
	int agent_position[2] = { ay, ax };  																							  // 2 entry array
	double epsilon;
	double alpha;
	double gamma;
	void init();
};

void agent::init() {
	ax = 0;  																														  // places learner at (0,0)
	ay = 0;
	epsilon = 0.3;
	alpha = 0.1;
	gamma = 0.9;
	agent_position[1] = ax;  																										  // stores agent's x coord in 2nd data entry
	agent_position[0] = ay;  																										  // stores agent's y coord in 1st data entry
}



/////////////////////////////////////////////////
///////////////// DOMAIN STUFF //////////////////
/////////////////////////////////////////////////



class domain {
public:
	int x;
	int y;
	void init();
	void write();
	void show(agent* plearner);
	void position(domain* pgrid, agent* plearner);
	void goal(domain* pgrid);
	int goal_x;
	int goal_y;
	vector<vector<int>> matrix;  																									  // 2D vector
	vector<vector<double>> qtable;   																								 // 2D vector
	vector<int> state;
	vector<int> rewards;
	void fillq(agent* plearner);
	void express(agent* plearner);
	void displayq();
	int goal_reward = 100;
};

void domain::init() {
	cout << "Please choose how many rows \n x = " << endl;
	cin >> x; 																											  // user input for rows
	cout << "Please choose how many columns \n y = " << endl;
	cin >> y; 																											  // user input for columns
	matrix.resize(x);  																									  // creates x columns (i think)
	goal_x = x - 1;  																										  // goal placed at a X coordinate
	goal_y = y - 1;  																										  // goal placed at a Y coordinate
	for (int i = 0; i < x; i++) {
		matrix[i].resize(y);  																							  // creates y rows (i think)

	}
	for (int t = 0; t < x; t++) {  																					 // creates state vector
		for (int j = 0; j < y; j++) {
			matrix[t][j] = j + (t*y);
			state.push_back(matrix[t][j]);   																			  // fills state 0 to x*y-1
		}
	}
	//for (int i = 0; i < state.size(); i++) {
	//	cout << state.at(i);  																							  // fills state vector
	//}
	for (int t = 0; t < x; t++) {
		for (int j = 0; j < y; j++) {
			matrix[t][j] = j + (t*y);
			rewards.push_back(matrix[t][j]);  																			  // creates rewards vector
		}
	}
	for (int i = 0; i < rewards.size(); i++) {
		rewards.at(i) = -0.99;  																								  // all rewards = -0.99
	}
	rewards.at(state.at(matrix[goal_x][goal_y])) = goal_reward;  																	  // goal reward
	cout << endl;

}
// obtained from http://stackoverflow.com/questions/1403150/how-do-you-dynamically-allocate-a-matrix



void domain::write() {
	for (int t = 0; t < x; t++) {
		for (int j = 0; j < y; j++) {
			matrix[t][j] = j + t*x;    																					  // populates state value matrix
		}
	}
	cout << endl;
}

void domain::show(agent* plearner) {
	for (int o = 0; o < x; o++) {
		for (int p = 0; p < y; p++) {
			cout << matrix[o][p] << "\t"; 																						  // show matrix
		}
		cout << endl;
	}
	cout << "\nYour X coordinate is " << plearner->ax << " and your Y coordinate is " << plearner->ay << ".\n" << endl; 			  // display learner coordinates
	cout << "The goal's coordinates are at " << goal_y << "," << goal_x << ".\n" << endl;  										  // display goal coordinates
}

void domain::express(agent* plearner) {
	cout << "\nYour X coordinate is " << plearner->ax << " and your Y coordinate is " << plearner->ay << ".\n" << endl; 			  // display learner coordinates
	cout << "The goal's coordinates are at " << goal_y << "," << goal_x << ".\n" << endl;  										  // display goal coordinates
}

void domain::position(domain* pgrid, agent* plearner) { 																			  // show where the agent is on grid
	int a = plearner->agent_position[0];
	int b = plearner->agent_position[1];
	int AP = 9;  																													  // agent represented as a 9 on the board
																																	  /*pgrid->matrix[a][b] = AP;*/  																								  // places agent representation on board
}

void domain::goal(domain* pgrid) { 																								  // show where goal is on grid
	int goal = 5;
	/*pgrid->matrix[goal_y][goal_x] = goal;*/  																					  // places goal coords onto gridworld
}

void domain::fillq(agent* plearner) {
	int st = x * y;
	qtable.resize(st);  																									  		  // creates x columns (i think)
	for (int i = 0; i < st; i++) {
		qtable[i].resize(4);  																									  // creates y rows (i think)
	}
	double q_init = 0;   																											 // initialize small q values for q table
	for (int i = 0; i < st; i++) {
		for (int j = 0; j < 4; j++) {
			q_init = 0.001 * ATRAND;
			qtable[i][j] = q_init;
		}
	}
	/*cout << "filled table" << endl;*/
	//for (int o = 0; o < st; o++) {
	//	for (int p = 0; p < 4; p++) {
	//		cout << qtable[o][p] << "\t"; 																						  // show q table
	//	}
	//	cout << endl;
	//}
	int state_current;
	state_current = state.at(matrix[plearner->ay][plearner->ax]);
	//cout << state.at(matrix[plearner->ay][plearner->ax]) << endl;  												  // read state from agent coords
	//cout << qtable[state_current][0] << endl;  																	  // read q value from state and action
}

void domain::displayq() {
	int st = x*y;
	for (int o = 0; o < st; o++) {
		for (int p = 0; p < 4; p++) {
			cout << qtable[o][p] << "\t 	\t"; 																						  // show q table
		}
		cout << endl;
	}
}


/////////////////////////////////////////////////
/////////////// MOVEMENT FUNCTIONS //////////////
/////////////////////////////////////////////////



void up(agent* plearner) {
	plearner->ay = plearner->ay - 1;    																							  // agent's y position is moved up 1
	plearner->agent_position[0] = plearner->ay;
	/*cout << "moved up" << endl;*/
};

void down(agent* plearner) {
	plearner->ay = plearner->ay + 1;    																							  // agent's y position is moved down 1
	plearner->agent_position[0] = plearner->ay;
	/*cout << "moved down" << endl;*/
}


void right(agent* plearner) {
	plearner->ax = plearner->ax + 1;    																							  // agent's x position is moved right 1
	plearner->agent_position[1] = plearner->ax;
	/*cout << "moved right" << endl;*/
}

void left(agent* plearner) {
	plearner->ax = plearner->ax - 1;    																							  // agent's x position is moved left 1
	plearner->agent_position[1] = plearner->ax;
	/*cout << "moved left" << endl;*/
}

void movement(agent*plearner) {
	if (plearner->skip_move == 0) {
		if (plearner->direction == 0) {
			up(plearner);
			/*cout << "up" << endl;*/
		}
		else if (plearner->direction == 1) {
			down(plearner);
			/*cout << "down" << endl;*/
		}
		else if (plearner->direction == 2) {
			left(plearner);
			/*cout << "left" << endl;*/
		}
		else if (plearner->direction == 3) {
			right(plearner);
			/*cout << "right" << endl;*/
		}
	}
}



void bumper(agent* plearner, domain* pgrid) {
	plearner->skip_move = 0;
	// edge cases
	if (plearner->ay == 0) {
		if (plearner->direction == 0) {																														// at top, trying to move up
			plearner->skip_move = 1;
			/*cout << "bumped down" << endl;*/
		}
	}
	if (plearner->ay == pgrid->y - 1) {																														// at bottom, trying to move down
		if (plearner->direction == 1) {
			plearner->skip_move = 1;
			/*cout << "bumped up" << endl;*/
		}
	}
	if (plearner->ax == 0) {																																// at left edge, trying to move left
		if (plearner->direction == 2) {
			plearner->skip_move = 1;
			/*cout << "bumped right" << endl;*/
		}
	}
	if (plearner->ax == pgrid->x - 1) {																														// at right edge, trying to move right
		if (plearner->direction == 3) {
			plearner->skip_move = 1;
			/*cout << "bumped left" << endl;*/
		}
	}
	// corner cases
	if (plearner->ay == 0 && plearner->ax == 0) {
		if (plearner->direction == 0 || plearner->direction == 2) {
			plearner->skip_move = 1;
			/*cout << "top left corner" << endl;*/
		}
	}
	if (plearner->ay == pgrid->y - 1 && plearner->ax == 0) {
		if (plearner->direction == 1 || plearner->direction == 2) {
			plearner->skip_move = 1;
			/*cout << "bottom left corner" << endl;*/
		}
	}
	if (plearner->ay == 0 && plearner->ax == pgrid->x - 1) {
		if (plearner->direction == 0 || plearner->direction == 3) {
			plearner->skip_move = 1;
			/*cout << "top right corner" << endl;*/
		}
	}
	if (plearner->ay == pgrid->y - 1 && plearner->ax == pgrid->x - 1) {
		if (plearner->direction == 1 || plearner->direction == 3) {
			plearner->skip_move = 1;
			/*cout << "bottom right corner" << endl;*/
		}
	}
}

int wall1(agent* plearner, domain* pgrid, int currentstate) {
	if (currentstate == 0 && plearner->direction == 3) {
		plearner->skip_move = 1;
		cout << "walled (right)" << endl;
	}
	if (currentstate == (pgrid->x + 1) && plearner->direction == 0) {
		plearner->skip_move = 1;
		cout << "walled (up)" << endl;
	}
	if (currentstate == pgrid->x - (pgrid->x - 2) && plearner->direction == 2) {
		plearner->skip_move = 1;
		cout << "walled (left)" << endl;
	}
	return plearner->skip_move;
}



/////////////////////////////////////////////////
///////// MOVING AGENT BACK TO BOARD FROM ///////
///////// EXTREEEEEEEEEEEME COORDINATES /////////
/////////////////////////////////////////////////



void testA(agent* plearner, domain* pgrid) {
	cout << "Enter an extreme X,Y coordinate" << endl;
	cout << "X coordinate =" << endl;
	cin >> plearner->ax;
	plearner->agent_position[1] = plearner->ax;
	cout << "Y coordinate =" << endl;
	cin >> plearner->ay;
	plearner->agent_position[0] = plearner->ay;
	cout << "\nExtreme coordinates are " << plearner->ax << "," << plearner->ay << "\n" << endl;
	if (plearner->ax > pgrid->x) {  																								  // reposition learner's x coord
		plearner->ax = 0;
		plearner->agent_position[1] = plearner->ax;
	}
	if (plearner->ay > pgrid->y) {  																								  // reposition learner's y coord
		plearner->ay = 0;
		plearner->agent_position[0] = plearner->ay;
	}
	cout << "Magic happens. \n" << endl;
	cout << "New coordinates are " << plearner->ax << "," << plearner->ay << "\n" << endl;
	pgrid->goal(pgrid);
	pgrid->position(pgrid, plearner); 																							  // positions the learner
	pgrid->write();
	pgrid->position(pgrid, plearner);
	pgrid->show(plearner);
}


/////////////////////////////////////////////////
//////////// HEURISTIC APPROACH /////////////////
/////////////////////////////////////////////////

int testC(agent* plearner, domain*pgrid, int prev_state) {
	cout << "Running Test C: Heuristic Control" << endl;
	plearner->ax = 0;
	plearner->ay = 0;
	int stop = 10;
	int stepstaken = 0;
	plearner->skip_move = 0;
	while (stop < 100) {
		if (plearner->ax < pgrid->goal_x) {
			plearner->direction = 3;																								// go right!
			bumper(plearner, pgrid);
			wall1(plearner, pgrid, prev_state);
			movement(plearner);
			stepstaken++;
		}
		else if (plearner->ax > pgrid->goal_x) {
			plearner->direction = 2;
			bumper(plearner, pgrid);
			wall1(plearner, pgrid, prev_state);
			movement(plearner);
			stepstaken++;
		}
		else if (plearner->ay < pgrid->goal_y) {
			plearner->direction = 1;
			bumper(plearner, pgrid);
			wall1(plearner, pgrid, prev_state);
			movement(plearner);
			stepstaken++;
		}
		else if (plearner->ay > pgrid->goal_y) {
			plearner->direction = 0;
			bumper(plearner, pgrid);
			wall1(plearner, pgrid, prev_state);
			movement(plearner);
			stepstaken++;
		}
	}
	return stepstaken;
}


/////////////////////////////////////////////////
/////////////////////////////////////////////////
/////////////////////////////////////////////////

int decide(agent* plearner) {
	int action;
	double decision = ATRAND;
	if (1 - plearner->epsilon <= decision) {  																						 // compares if epsilon is less than or equal to a random number
		action = 1;  																												 // random action
	}
	else {
		action = 2;  																												 // greedy action
	}
	return action;
}

double act1(agent* plearner, domain* pgrid, int prev_state) {  																					 // random action
																																				 /*cout << "random" << endl;*/
	plearner->direction = rand() % 4;
	if (plearner->direction == 0) {   																								 // random up
		bumper(plearner, pgrid);
		wall1(plearner, pgrid, prev_state);
		movement(plearner);
	}
	if (plearner->direction == 1) {   																								 // random down
		bumper(plearner, pgrid);
		wall1(plearner, pgrid, prev_state);
		movement(plearner);
	}
	if (plearner->direction == 2) {   																								 // random left
		bumper(plearner, pgrid);
		wall1(plearner, pgrid, prev_state);
		movement(plearner);
	}
	if (plearner->direction == 3) {   																								 // random right
		bumper(plearner, pgrid);
		wall1(plearner, pgrid, prev_state);
		movement(plearner);
	}

	return plearner->direction;
}

double act2(agent* plearner, domain* pgrid, int prev_state) {  																					 // greedy action
																																				 /*cout << "greedy" << endl;*/
	double Q_up;
	double Q_down;
	double Q_right;
	double Q_left;
	int state_current;
	state_current = pgrid->state.at(pgrid->matrix[plearner->ay][plearner->ax]);   					 // checks the state vector based on state matrix coords
	Q_up = pgrid->qtable[state_current][0];   														 // Q for up at current
	Q_down = pgrid->qtable[state_current][1];   													 // Q for down at current
	Q_right = pgrid->qtable[state_current][3];   													 // Q for left at current
	Q_left = pgrid->qtable[state_current][2];   													 // Q for right at current
	double findmax[4] = { Q_up, Q_down, Q_right, Q_left };
	double max = 0;   																																		 // find max q of current state
	for (int i = 0; i < 5; i++) {
		if (findmax[i] > max) {
			max = findmax[i];
		}
	}
	// taken from http://www.cplusplus.com/forum/general/51452/
	/*cout << "greedy max = " << max << endl;*/
	if (max == Q_up) {
		plearner->direction = 0;
	}
	if (max == Q_down) {
		plearner->direction = 1;
	}
	if (max == Q_right) {
		plearner->direction = 3;
	}
	if (max == Q_left) {
		plearner->direction = 2;
	}
	if (plearner->direction == 0) {
		bumper(plearner, pgrid);
		wall1(plearner, pgrid, prev_state);
		movement(plearner);
	}
	if (plearner->direction == 1) {
		bumper(plearner, pgrid);
		wall1(plearner, pgrid, prev_state);
		movement(plearner);
	}
	if (plearner->direction == 2) {
		bumper(plearner, pgrid);
		wall1(plearner, pgrid, prev_state);
		movement(plearner);
	}
	if (plearner->direction == 3) {
		bumper(plearner, pgrid);
		wall1(plearner, pgrid, prev_state);
		movement(plearner);
	}
	return plearner->direction;
}

int current_state(agent* plearner, domain* pgrid) {
	int state_current;
	state_current = pgrid->state.at(pgrid->matrix[plearner->ay][plearner->ax]);																				// record state that learner is in
	return state_current;
}

// 0 1 2 3
// u d l r

double react(agent* plearner, domain* pgrid, int input, int prev_state) {
	int reward;
	double old_q;
	double curr_q;
	reward = pgrid->rewards.at(pgrid->state.at(pgrid->matrix[plearner->ay][plearner->ax]));																	// reward from movement
	if (input == 0) {
		old_q = pgrid->qtable[prev_state][input];   																										 // q from last state
		double Q_up;
		double Q_down;
		double Q_right;
		double Q_left;
		int state_current;
		state_current = pgrid->state.at(pgrid->matrix[plearner->ay][plearner->ax]);																			// post movement state
		if (state_current == prev_state) {																													// if a bumper is hit, reward = -1
			reward = -1;
		}
		/*cout << "state is " << state_current << endl;*/
		Q_up = pgrid->qtable[state_current][0];																												// post movement Q values
		Q_down = pgrid->qtable[state_current][1];
		Q_right = pgrid->qtable[state_current][3];
		Q_left = pgrid->qtable[state_current][2];
		/*cout << "max q's " << Q_up << " " << Q_down << " " << Q_left << " " << Q_right << endl;*/
		double findmax[4] = { Q_up, Q_down, Q_right, Q_left };
		double max = 0;   																																	 // post movement q max
		for (int i = 0; i < 4; i++) {
			if (findmax[i] > max) {
				max = findmax[i];
			}
		}
		/*cout << "max is " << max << endl;*/
		double update_q;
		update_q = old_q + plearner->alpha*(reward + (plearner->gamma*max) - old_q);																		// new q value for prev. movement
		pgrid->qtable[prev_state][input] = update_q;																										// places new q value for that state,action
																																							/*cout << "new q is " << update_q << "for state, action " << prev_state << " " << input << endl;*/
	}
	// repeat for other movements
	if (input == 1) {
		old_q = pgrid->qtable[prev_state][input];   																										 // q from last state
																																							 /*cout << "old q is " << old_q << endl;*/
		double Q_up;
		double Q_down;
		double Q_right;
		double Q_left;
		int state_current;
		state_current = pgrid->state.at(pgrid->matrix[plearner->ay][plearner->ax]);
		if (state_current == prev_state) {
			reward = -1;
		}
		/*cout << "state is " << state_current << endl;*/
		Q_up = pgrid->qtable[state_current][0];
		Q_down = pgrid->qtable[state_current][1];
		Q_right = pgrid->qtable[state_current][3];
		Q_left = pgrid->qtable[state_current][2];
		/*cout << "max q's " << Q_up << " " << Q_down << " " << Q_left << " " << Q_right << endl;*/
		double findmax[4] = { Q_up, Q_down, Q_right, Q_left };
		double max = 0;   																																	 // current q max
		for (int i = 0; i < 4; i++) {
			if (findmax[i] > max) {
				max = findmax[i];
			}
		}
		/*cout << "max is " << max << endl;*/
		double update_q;
		update_q = old_q + plearner->alpha*(reward + (plearner->gamma*max) - old_q);
		pgrid->qtable[prev_state][input] = update_q;
		/*cout << "new q is " << update_q << "for state, action " << prev_state << " " << input << endl;*/
	}
	if (input == 3) {
		old_q = pgrid->qtable[prev_state][input];   																										 // q from last state
																																							 /*cout << "old q is " << old_q << endl;*/
		double Q_up;
		double Q_down;
		double Q_right;
		double Q_left;
		int state_current;
		state_current = pgrid->state.at(pgrid->matrix[plearner->ay][plearner->ax]);
		if (state_current == prev_state) {
			reward = -1;
		}
		/*cout << "state is " << state_current << endl;*/
		Q_up = pgrid->qtable[state_current][0];
		Q_down = pgrid->qtable[state_current][1];
		Q_right = pgrid->qtable[state_current][3];
		Q_left = pgrid->qtable[state_current][2];
		/*cout << "max q's " << Q_up << " " << Q_down << " " << Q_left << " " << Q_right << endl;*/
		double findmax[4] = { Q_up, Q_down, Q_right, Q_left };
		double max = 0;   																																	 // current q max
		for (int i = 0; i < 4; i++) {
			if (findmax[i] > max) {
				max = findmax[i];
			}
		}
		/*cout << "max is " << max << endl;*/
		double update_q;
		update_q = old_q + plearner->alpha*(reward + (plearner->gamma*max) - old_q);
		pgrid->qtable[prev_state][input] = update_q;
		/*cout << "new q is " << update_q << "for state, action " << prev_state << " " << input << endl;*/
	}
	if (input == 2) {
		old_q = pgrid->qtable[prev_state][input];
		/*cout << "old q is " << old_q << endl;*/
		double Q_up;
		double Q_down;
		double Q_right;
		double Q_left;
		int state_current;
		state_current = pgrid->state.at(pgrid->matrix[plearner->ay][plearner->ax]);
		if (state_current == prev_state) {
			reward = -1;
		}
		/*cout << "state is " << state_current << endl;*/
		Q_up = pgrid->qtable[state_current][0];
		Q_down = pgrid->qtable[state_current][1];
		Q_right = pgrid->qtable[state_current][3];
		Q_left = pgrid->qtable[state_current][2];
		/*cout << "max q's " << Q_up << " " << Q_down << " " << Q_left << " " << Q_right << endl;*/
		double findmax[4] = { Q_up, Q_down, Q_right, Q_left };
		double max = 0;   																																	 // current q max
		for (int i = 0; i < 4; i++) {
			if (findmax[i] > max) {
				max = findmax[i];
			}
		}
		/*cout << "max is " << max << endl;*/
		double update_q;
		update_q = old_q + plearner->alpha*(reward + (plearner->gamma*max) - old_q);
		pgrid->qtable[prev_state][input] = update_q;
		/*cout << "new q is " << update_q << "for state, action " << prev_state << " " << input << endl;*/
	}
	return 0;
}

int main()
{
	srand(time(NULL));
	cout << "Coordinates start from the top left corner. \nThe AGENT is represented by a 9. \nThe GOAL is represented by a 5. \nAn empty space is represented by a 1." << endl;
	agent learner;
	learner.init();
	agent* plearner = &learner;
	domain grid;
	grid.init();

	domain* pgrid = &grid;
	grid.goal(pgrid); 																												 // establishes goal
	grid.position(pgrid, plearner); 																								 // positions the learner
	grid.show(plearner);
	cout << "\n" << endl;
	grid.fillq(plearner);

	int input = 99;																													// place holder
	int prev_state;
	int check = 10;																													// place holder
	double steps = 0;
	int greedy = 0;																													// greedy action counter
	int random = 0;																													// random action counter
	int iterations = 1000;																											// one run = X amount of iterations
	vector<int> stepvector;																											// count steps
	int st = grid.x * grid.y;																										// to be used for test F
	double edc = -0.000001;																											// decay constant for epsilon
	int runs = 30;
	for (int h = 0; h < runs; h++) {
		for (int i = 0; i < iterations; i++) {
			check = 10;
			while (check < 100) {																										// while loop will stop when agent reaches goal
				prev_state = current_state(plearner, pgrid);																			// state before movement
																																		/*cout << "prev state is " << prev_state << endl;*/
				int action = decide(plearner);																							// decide what to do
				if (action == 1) {																										// will do random action
					input = act1(plearner, pgrid, prev_state);																						// random action
					steps++;																											// increase step count
					random++;																											// increase random count
				}
				if (action == 2) {
					input = act2(plearner, pgrid, prev_state);																						// greedy action
					steps++;																											// increase step count
					greedy++;																											// increase random count
				}
				react(plearner, pgrid, input, prev_state);																				// will update q value
																																		// TEST D
				if (learner.ax == grid.goal_x && learner.ay == grid.goal_y) {															// checks if agent is at goal
					check = 200;																										// end while loop
																																		// TEST F
					if (steps < (grid.x + grid.y) - 2) {
						assert(1 == 9);
					}
					stepvector.push_back(steps);																						// add entry to step vector
																																		// TEST E
					steps = 0;
					learner.ax = 0;
					learner.ay = 0;
					learner.epsilon = learner.epsilon * pow(2.7181818, edc*i);															// decay epsilon upon iteration ending
					for (int i = 0; i < st; i++) {
						for (int j = 0; j < 4; j++) {
							if (grid.qtable[i][j] > grid.goal_reward + 0.001) {															// end program if goal reward is exceeded
								cout << grid.qtable[i][j] << endl;																		// +0.001 due to glitches?
								assert(1 == 9);
							}
						}	//end goal reward check
					}	//end test E
				}	//end test D
			}	//end 1 iteration
		}	//end 1000 iterations
		grid.fillq(plearner);
	}	//end runs
	grid.displayq();
	ofstream outFile;   																									 // output file
	outFile.open("Ta_Alton_493_ProjectBeta.csv");   																		 // name of output file
	outFile << 1 << "\n" << endl;
	for (int w = 0; w < iterations*runs; w++) {
		outFile << stepvector.at(w) << ",";   																// outputs episodes/iterations and its corresponding steps to a text file
		if ((w - (iterations - 1)) % (iterations) == 0 && w != 0 && w != 1) {
			outFile << endl;
		}
	}
	outFile.close();
	int begin;
	cout << "Running Test C: Heuristic Control. Press a number to begin. \nThe test will end when the agent has taken 25001 steps, as it is unlikely (unless through a glitch) that the agent will reach the goal." << endl;
	cin >> begin;
	plearner->ax = 0;
	plearner->ay = 0;
	int stop = 10;
	int stepstaken = 0;
	plearner->skip_move = 0;
	prev_state = current_state(plearner, pgrid);
	while (stop < 100) {
		if (plearner->ax < pgrid->goal_x) {
			plearner->direction = 3;																								// go right!
			bumper(plearner, pgrid);
			wall1(plearner, pgrid, prev_state);
			movement(plearner);
			stepstaken++;
		}
		else if (plearner->ax > pgrid->goal_x) {
			plearner->direction = 2;
			bumper(plearner, pgrid);
			wall1(plearner, pgrid, prev_state);
			movement(plearner);
			stepstaken++;
		}
		else if (plearner->ay < pgrid->goal_y) {
			plearner->direction = 1;
			bumper(plearner, pgrid);
			wall1(plearner, pgrid, prev_state);
			movement(plearner);
			stepstaken++;
		}
		else if (plearner->ay > pgrid->goal_y) {
			plearner->direction = 0;
			bumper(plearner, pgrid);
			wall1(plearner, pgrid, prev_state);
			movement(plearner);
			stepstaken++;
		}
		else if (plearner->ax == pgrid->goal_x && plearner->ay == pgrid->goal_y) {													// end while loop
			stop = 200;
		}
		if (stepstaken > 25000) {
			cout << "Steps = " << stepstaken << endl;
			cout << "\nYour X coordinate is " << plearner->ax << " and your Y coordinate is " << plearner->ay << ".\n" << endl; 			  // display learner coordinates
			cout << "The goal's coordinates are at " << pgrid->goal_y << "," << pgrid->goal_x << ".\n" << endl;  										  // display goal coordinates
			if (plearner->ax != pgrid->goal_x && plearner->ay != pgrid->goal_y) {
				cout << "The heuristic agent has not reached the goal in 25001 steps. It has failed." << endl;
			}
			if (plearner->ax == pgrid->goal_x && plearner->ay == pgrid->goal_y) {
				cout << "Call Father Damien because this agent needs an exorcism performed." << endl;
			}
			cout << "Ending heuristic control. Steps taken > 25000." << endl;
			break;
		}
	}
	cout << "Snake has escaped from New York! \n" << endl;
	return 0;
}
