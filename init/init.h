
struct queue {
	UINT8 pid [6];
	UINT32* current;
	UINT32* end; //loc to add next val
}

//Built based on structure from test dummy procs
struct PCB {
	UINT16 pws;
	
	UINT32* pc;	
	UINT32* stack;

	PCB* next;
	int id;
	int state;			
	int priority;
	bool returning;
}

void put_to_ready(PCB* p);
void put_to_blocked(int waiting_on, PCB* p);
void remove_from_ready(PCB* p);
void remove_from_blocked(PCB* p);

UINT32 pop (struct process p);
void push (struct process p, UINT32 val);

void init ();
