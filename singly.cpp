
class singly
{
	public:
		vertex* createVertex(GLfloat xCoord, GLfloat yCoord);
		void append(vertex *node);
		void deleteVertex(vertex *node);
		void deleteTheWholeDamnThing();
		int getLength();
		vertex *head;
		singly()
		{
			head=NULL;
		}
};

int singly::getLength()
{
	int i=0;
	struct vertex *currentVert=head;

	while (currentVert != NULL)
	{
		currentVert=currentVert->next;
		i++;
	}
}

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
}

void singly::deleteVertex(vertex *node)
{
	struct vertex *temp=head;
	if (head==node)
	{
		if(head->next!=NULL)
		{
			temp = head;
			head=head->next;
			delete temp;
			delete node;
		}
		else if(head->next==NULL)
			head=NULL;
	}
	else
	{
		while(temp->next != node)
		{
			if(temp->next==NULL)
			{
				cout << "error in search for deletion. 91" << endl;
			}
			temp=temp->next;
		}
		temp->next=temp->next->next;
		delete node;
		return;
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

}