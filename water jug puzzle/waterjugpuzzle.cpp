/*******************************************************************************
 * Name        : waterjugpuzzle.cpp
 * Author      : Conor McGullam
 * Date        : 3/13/2020
 * Description : Pours virtual water into virtual jugs.
 * Pledge      : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/
#include <iostream>
#include <sstream>
#include <stack>
#include <queue>

using namespace std;

// Struct to represent state of water in the jugs.
struct State {
    int a, b, c;
    string directions;
    State *parent;
    
    State(int _a, int _b, int _c, string _directions) : 
        a{_a}, b{_b}, c{_c}, directions{_directions}, parent{nullptr} { }
    
    // String representation of state in tuple form.
    string to_string() {
        ostringstream oss;
        oss << "(" << a << ", " << b << ", " << c << ")";
        return oss.str();
    }
};

void jug_printer(State * last) {
	/* Loops through parent pointers to put the chain of States that make the solution into a stack.
	 * Then pops all States from the stack and prints them. */
	stack<State*> callbacks;
	State * curr = last;
	while(curr->parent != nullptr) {
		callbacks.push(curr);
		curr = curr->parent;
	}
	cout << curr->directions << " " << curr->to_string() << endl;
	while(!callbacks.empty()) {
		curr = callbacks.top();
		callbacks.pop();
		cout << curr->directions << " " << curr->to_string() << endl;
	}
}

void jug_solver(int capA, int capB, int capC, int goalA, int goalB, int goalC) {
	//building 2d array of pointers
	State ***visited = new State**[capA+1];
	for(int i = 0; i < capA+1; ++i) {
		visited[i] = new State*[capB+1];
		for(int j = 0; j < capB+1; ++j) {
			visited[i][j] = nullptr;
		}
	}
	//setting root of tree
	visited[0][0] = new State(0, 0, capC, "Initial state.");
	//will eventually hold the solution
	State * ans = visited[0][0];
	//queue holds 'row' of tree which allows for BFS
	queue<State*> row;
	//add root to row
	row.push(visited[0][0]);
	
	
	while(!row.empty()) {
		//all of the work is done on jugs. Jugs is set as first elements in 'row' queue.
		State * jugs = new State((row.front())->a, (row.front())->b, (row.front())->c, (row.front())->directions);
		
		/* Need copy of jugs for setting parent and for reverting jugs back to previous state
		 * in next iteration of loop. */
		State * previous = visited[jugs->a][jugs->b];
		
		//used to do the 6 ways to pour by manipulating indexes
		int * seq[3][2] = { {&jugs->a, &capA}, {&jugs->b, &capB}, {&jugs->c, &capC} };
		row.pop();
		
		int i = 0;
		for(int j = 0; j < 6; ++j) {
			jugs->a = previous->a;
			jugs->b = previous->b;
			jugs->c = previous->c;
			jugs->directions = previous->directions;
			//used when creating 'directions' string
			int poured = 0;
			
			/* Sets i index with some spaghetti because I couldn't think of a better way.
			 * i will be 2, 1, 2, 0, 1, 0 in the 6 pour cycle to match with jugs C, B, C, A, B, A. */
			if(j == 0) {
				i = 2;
			}else if(j == 3) {
				i = i - 2;
			}else if(j % 2 == 0) {
				++i;
			}else {
				--i;
			}
			
			/*j is solely used to set k. k will be 0, 0, 1, 1, 2, 2 in the 6 pour cycle to match up with
			 * jugs A, A, B, B, C, C. Using respective i and k in each iteration for pouring will get
			 * C to A, B to A, C to B, A to B, B to C, A to C. */
			int k = j/2;
			
			//if jug1 is not full and jug2 is not empty			
			if(*seq[k][0] != *seq[k][1] && *seq[i][0] != 0) {
				//if jug1 + jug2 > cap1 then fill jug1 and leave remainder in jug2
				if(*seq[k][0] + *seq[i][0] > *seq[k][1]) {
					*seq[i][0] = *seq[i][0] - (*seq[k][1] - *seq[k][0]);
					poured = *seq[k][1] - *seq[k][0];
					*seq[k][0] = *seq[k][1];
				//else add all of jug2 to jug1 and empty jug2
				}else {
					*seq[k][0] = *seq[k][0] + *seq[i][0];
					poured = *seq[i][0];
					*seq[i][0] = 0;
				}
			}
			//if node has not been visited yet
			if(visited[jugs->a][jugs->b] == nullptr) {
				//building pour string
				string ichar, kchar;
				ichar = (char)(i+65);
				kchar = (char)(k+65);
				string pour;
				if(poured == 1) {
					pour = "Pour " + to_string(poured) + " gallon from " + ichar + " to " + kchar + ".";
				}else {
					pour = "Pour " + to_string(poured) + " gallons from " + ichar + " to " + kchar + ".";
				}
				//creating copy of jugs in array and setting pointer to parent
				visited[jugs->a][jugs->b] = new State(jugs->a, jugs->b, jugs->c, pour);
				visited[jugs->a][jugs->b]->parent = previous;
				//checking if we've reached the solution
				if(jugs->a == goalA && jugs->b == goalB) {
					ans = visited[jugs->a][jugs->b];
					delete jugs;
					goto done;
				}else {
					//adds unique jug permutation to row
					row.push(visited[jugs->a][jugs->b]);
				}
			}
		}
		delete jugs;
	}
	done:
	
	/* If ans hasn't changed and the goal isn't the initial state it prints 'No solution.'
	 * Else prints solution. */
	if((ans->a == 0 && goalA != 0) || (ans->b == 0 && goalB != 0)) {
		cout << "No solution." << endl;
	}else {
		jug_printer(visited[ans->a][ans->b]);
	}

	//delete
	for(int i = 0; i < capA+1; ++i) {
		for(int j = 0; j < capB+1; ++j) {
			delete visited[i][j];
		}
		delete [] visited[i];
	}
	delete [] visited;
}

int main(int argc, char * const argv[]) {
	int capA, capB, capC, goalA, goalB, goalC;
	//used for error message
	int * temp[6] = { &capA, &capB, &capC, &goalA, &goalB, &goalC };
	if(argc != 7) {
		cerr << "Usage: ./waterjugpuzzle <cap A> <cap B> <cap C> <goal A> <goal B> <goal C>" << endl;
		return 1;
	}else {
		istringstream iss;
		/* Dynamically builds invalid capacity/goal error messages based on which value
		 * has the error. */
		for(int i = 1; i < 7; ++i) {
			iss.str(argv[i]);
			if (!(iss >> *temp[i-1]) || (i <= 3 && *temp[i-1] <= 0) || (i > 3 && *temp[i-1] < 0)) {
				cerr << "Error: Invalid ";
				if(i < 4) {
					cerr << "capacity ";
				}else {
					cerr << "goal ";
				}
				cerr << "'" << argv[i] << "' for jug " << (char)(((i-1)%3)+65) << "." << endl;
				return 1;
			}
			iss.clear();
		}
		
		//other errors
		if(goalA > capA) {
			cerr << "Error: Goal cannot exceed capacity of jug A." << endl;
			return 1;
		}else if(goalB > capB) {
			cerr << "Error: Goal cannot exceed capacity of jug B." << endl;
			return 1;
		}else if(goalC > capC) {
			cerr << "Error: Goal cannot exceed capacity of jug C." << endl;
			return 1;
		}else if(goalA + goalB + goalC != capC) {
			cerr << "Error: Total gallons in goal state must be equal to the capacity of jug C." << endl;
			return 1;
		}else {
			//solves jugs
			jug_solver(capA, capB, capC, goalA, goalB, goalC);
			return 0;
		}
	}
}
