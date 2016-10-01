
class singly
{
	public:
		vertex* createVertex(GLfloat xCoord, GLfloat yCoord);
		void append(vertex *node);
		void deleteVertex(vertex *node);
		void deleteTheWholeDamnThing();
		int getLength();
		void printList();
		vertex *head;
		singly()
		{
			head = new struct vertex;
			head=NULL;
			length=0;
		}
	private:
		int length;
};

int singly::getLength()
{
	
	length=0;
	vertex *county = head;
	while (county!=NULL)
	{
		county=county->next;
		length++;
	}
	return length;
}
//
vertex* singly::createVertex(GLfloat xCoord, GLfloat yCoord)
{
	struct vertex *temp;
	temp = new(struct vertex);
	if(temp==NULL)
	{
		cout << "catastrophe in creation. line 38" << endl;
		return 0;
	}
	else
	{
		temp->x=xCoord;
		temp->y=yCoord;
		temp->next=NULL;
		return temp;
	}
}
void singly::append(vertex *node)
{
	struct vertex *tempStart;
	tempStart=head;
	if(head==NULL)
	{//if list is empty aka this is first entry
		head=node;
		head->next=NULL;
		cout << "NEW HEAD IS " << node->x << " " << node->y << endl;
	}
	else
	{//otherwise...
		while(tempStart->next != NULL)//until you get to last node
		{//move down the list one by one
			 tempStart=tempStart->next;
		}//when you get to last node...
		//ensure new node's next is NULL to be safe or something
		node->next=NULL;
		//append new node to last node in list
		tempStart->next=node;
		cout << "APPENDED " << node->x << " " << node->y << endl;//announce it for the world to see
	}
	length++;
}

void singly::deleteVertex(vertex *node)
{

	if (getLength()==0)
		return;
	else if (head==node) 
	{
		if(head->next==NULL)
		{
			delete node;
			head=new struct vertex;
			return;
		}
		
		head->x=head->next->x;
		head->y=head->next->y;
		node = head->next;
		head->next=head->next->next;
		node=NULL;
		delete node;

		length--;
	}
	else 
	{
		struct vertex *lastVert = head;
		while(lastVert->next!=node && lastVert->next!=NULL)
			lastVert=lastVert->next;

		if (lastVert->next==NULL)
			return;

		lastVert->next=lastVert->next->next;
		node=NULL;
		delete node;
		length--;
	}
}
void singly::deleteTheWholeDamnThing()
{
	vertex *deadManWalking=head;
	while(head!=NULL)
	{
		head=head->next;
		deadManWalking=NULL;
		delete deadManWalking;
	}
	length=0;
}
void singly::printList()
{
	vertex *printy=head;
	cout << "\n------------------------\nhead\n";
	while(printy!=NULL)
	{
		cout<<printy->x<<", "<<printy->y<<endl;
		printy=printy->next;
	}
	cout<<"----------------------------\n";
}
