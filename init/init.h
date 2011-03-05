
struct queue {
	UINT8 pid [6];
	UINT32* current;
	UINT32* end; //loc to add next val
}

//Built based on structure from test dummy procs
struct PCB {
	PCB* next;
	UINT8* id;
	UINT8* state;			
	UINT8* priority;
	UINT32* pc;	//not sure about the variable type for this
	UINT32* stack;
}

void put_to_ready(PCB* p);
void put_to_blocked(int waiting_on, PCB* p);
void remove_from_ready(PCB* p);
void remove_from_blocked(PCB* p);

UINT32 pop (struct process p);
void push (struct process p, UINT32 val);

void init ();
