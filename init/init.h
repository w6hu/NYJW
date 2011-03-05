
struct queue {
	UINT8 pid [6];
	UINT32* current;
	UINT32* end; //loc to add next val
}

//Built based on structure from test dummy procs
struct process {
	UINT8* id;
	UINT8* state;			
	UINT8* priority;
	UINT32* pc;	//not sure about the variable type for this
	UINT32* stack;
}

UINT8 next (struct queue q);
void add (struct queue q, UINT8* pid);

UINT32 pop (struct process p);
void push (struct process p, UINT32 val);

void init ();
