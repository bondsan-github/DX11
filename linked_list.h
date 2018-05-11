#pragma once

template< typename type >
class node
{
public:

	node( type data , node< type > * const previous = nullptr , node< type > * const next = nullptr )
	:m_data( data ) , m_previous( previous ) , m_next( next ) {}

	void previous( node< type > * const previous ) { m_previous = previous; }
	void next( node< type > * const next ) { m_next = next; }

	node * previous() { return m_previous; }
	node * next() { return m_next; }

	type data() const { return m_data; }

private:

	type m_data;
	node * m_previous	= nullptr;
	node * m_next		= nullptr;
};

// linked_list cyclic_list // { 1, 2, 3, 4 };
// cyclic_list.add( 1 );

template< typename type >
class linked_list
{
public:	
	//linked_list();
	~linked_list();

	void add( type in_type );		// adds item to end of list
	//void	add( type ... );
	//void	add_front( node * in_node );
	
	//void 	remove_at( const unsigned int index );
	//void 	del_first();
	//void	del_last();
	//bool	clear();

	type first() 
	{ 
		if( m_first ) return m_first->data(); 
		else return type();
		//else throw "list has no elements";
	}
	//type	last() { return m_last->data(); }	

	type current()
	{
		return m_iterator->data();
	}

	void increment() 
	{ 	
		m_iterator = m_iterator->next();
	}

	void decrement()
	{
		m_iterator = m_iterator->previous();
	}

	type at( const unsigned int index );
	//int count() const { return m_count; }
	

	//operator {}
	//opertator []
	//operator ++ ( m_iterator = m_iterator->next() )
	//operator --
	
private:
		
	node< type >	* m_first		= nullptr;
	node< type >	* m_last		= nullptr;
	node< type >	* m_iterator	= nullptr;

	unsigned int m_count = 0;
};	

//-----------------------------------------------------------
// Destructor
//-----------------------------------------------------------
template< typename type >
linked_list< type >::~linked_list(void)
{
	/*
	type *p_current	= NULL;
	type *p_next	= NULL;

	if(m_pFirst != NULL) //if(m_pFirst)
	{
		p_current = m_pFirst;	

		while(p_current->next() != NULL)	// while(p_current->next())  // []->[]->[]
		{
			p_next = p_current->next();
			delete p_current;
			p_current = p_next;
			
			m_iNodeCount--;
		}
		delete p_current;
		m_iNodeCount--;
	} 
	//errorlog.write("\n node.destructor: nodecount %d", m_iNodeCount);
	*/
}
//-----------------------------------------------------------
// add a node at end of list
//-----------------------------------------------------------
template< typename type >
void linked_list< type >::add( type in_item )
{
	//no previous nodes 
	if( m_first == nullptr)
	{
		m_first = new node< type >( in_item );

		m_first->previous( m_first );
		m_first->next( m_first );

		m_last = m_first;
	}
	//add node to end of list
	else
	{
		node< type > * new_node = new node< type >( in_item ); // , m_last , nullptr );
	
		new_node->previous( m_last );
		new_node->next( m_first );

		m_last->next( new_node );
		m_first->previous( new_node );

		m_last = new_node;

	}
	m_iterator = m_first;
	++m_count;
}
//-----------------------------------------------------------
// remove all nodes
//-----------------------------------------------------------
/*
template< typename type >
bool linked_list<type>::clear(void)
{

	type *p_current = NULL;
	type *p_next = NULL;
	
	if(m_pFirst != NULL) 
	{
		p_current = m_pFirst;	

		while(p_current->next() != NULL)	 // []->[]->[]
		{
			p_next = p_current->next();
			delete p_current;
			p_current = p_next;

			m_iNodeCount--;
		}
		delete p_current;			  
		m_iNodeCount--;			
	*/
		/*while(p_current != NULL)	 // []->[]->[]
		{
			p_next = p_current->next();
			delete p_current;
			p_current = p_next;

			m_iNodeCount--;
		}*/

	/*	m_pFirst = NULL; m_pLast = NULL;
	}
	else if(m_pFirst == 0 && m_pLast == 0 && m_iNodeCount == 0) { return true; }
	else 
	{ 
		errorlog.write("node.del_all: not all nodes deleted. nodeCount=%d", m_iNodeCount);
		return false; 
	}
	return true;
}*/


//-----------------------------------------------------------
// del_pos
//-----------------------------------------------------------
//template< typename type >
//void linked_list< type >::remove_at( const unsigned int index )
//{
	//errorlog.write("\nnodes.del_pos: nodecount before delete=%d, pos=%d", m_iNodeCount, _pos);

	// if list not empty and pos within list node count range
	//if(m_pFirst != NULL && _pos >= 1 && _pos <= m_iNodeCount)
	//{	
	//	// if first node
	//	if(_pos == 1) 
	//	{
	//		type *pCurrent = m_pFirst;

	//		// only one node in list
	//		if(m_pFirst->next() == NULL)
	//		{
	//			delete m_pFirst;
	//			m_pFirst = NULL;

	//			m_iNodeCount--;
	//			return true;
	//		}
	//		// more than one node in list
	//		else
	//		{
	//			m_pFirst = m_pFirst->next();
	//			m_pFirst->setPrev(NULL);
	//			delete pCurrent;

	//			m_iNodeCount--;
	//			return true;
	//		}
	//		//errorlog.write("\ndel pos 1: node count: %d", m_iNodeCount);
	//	}
	//	// if last node
	//	// if(getPos(_pos) == m_pLast)
	//	else if(_pos == m_iNodeCount && m_pLast != NULL) //[]->[]
	//	{
	//		//errorlog.write("\nnode last: pos=%d", _pos);
	//		type *pCurrent = m_pLast;	
	//		//if(m_pFirst == m_pLast)
	//		if(m_pLast->prev() == NULL)
	//		{ 
	//			delete m_pLast;
	//			m_pFirst = NULL;
	//			m_pLast = NULL;

	//			m_iNodeCount--;
	//			return true;
	//		}
	//		else 
	//		{
	//			m_pLast = m_pLast->prev();
	//			m_pLast->setNext(NULL);
	//			delete pCurrent;

	//			m_iNodeCount--;
	//			return true;
	//		}
	//	} 
	//	// if a middle node
	//	//else if(
	//	else if(_pos > 1 && _pos < m_iNodeCount) // []->[]
	//	{
	//		//errorlog.write("\nnode middle: pos=%d", _pos);
	//		type *pCurrent = NULL; 
	//		//int	 nodeCount = 1;
	//		/*
	//		while(nodeCount < _pos && pCurrent->next() != NULL)
	//		{
	//			pCurrent = pCurrent->next(); //[]->[]->[]->[]
	//			nodeCount++;
	//		}*/
	//		pCurrent = getPos(_pos);
	//		if(pCurrent == NULL) { return false; }

	//		if(pCurrent->prev() != NULL && pCurrent->next() != NULL)
	//		{
	//			//	 |------\/
	//			//  []<-[]->[]
	//			pCurrent->prev()->setNext(pCurrent->next());
	//			//  \/------|
	//			//	[]  []->[]
	//			pCurrent->next()->setPrev(pCurrent->prev());
	//			delete pCurrent;

	//			m_iNodeCount--;			
	//			return true;
	//		} else { errorlog.write("\nnode.del_midd: next or prev = null"); return false; }
	//	} 
	//	else { errorlog.write("\nnode.del pos error"); }
	//} 

	//return false;
//}
//-----------------------------------------------------------
// getPos
//-----------------------------------------------------------
template< typename type >
type linked_list< type >::at( const unsigned int in_index )
{			
	unsigned int index = 0;

	if( in_index < 0 || in_index > m_count ) return type();
	else if( in_index == 0 ) return m_first->data();
	else if( in_index == m_count ) return m_last->data();
	else
	{
		while( index < in_index )
		{
			m_iterator = m_first->next();
			++index;
		}
	}
	
	return m_iterator->data();  
}

