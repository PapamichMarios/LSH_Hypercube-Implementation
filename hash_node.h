#ifndef __HASH_NODE_H__
#define __HASH_NODE_H__

template <typename K, typename V>
class HashNode
{
	private:
    	K key;
    	V value;
		/*== g*/
    	HashNode *next;

	public:
    	HashNode(const K &key, const V &value) 
			: key(key), value(value), next(NULL) { }
		
		~HashNode()
		{
			
		}

		K getKey()
		{
			return key;
		}

		V getValue()
		{
			return value;
		}

		void setValue(V value)
		{
			this->value = value;
		}

		HashNode *getNext()
		{
			return next;
		}

		void setNext(HashNode *next)
		{
			this->next = next;
		}

};
#endif
