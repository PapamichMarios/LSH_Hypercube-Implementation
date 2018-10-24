#ifndef __HASH_NODE_H__
#define __HASH_NODE_H__

template <typename K>
class HashNode
{
	private:
    	K key;
    	std::string G;
		HashNode *next;

	public:
    	HashNode(const K &key, const std::string G) : key(key), G(G), next(NULL) { }
		
		~HashNode()
		{
			
		}

		K getKey()
		{
			return this->key;
		}

		std::string getG()
		{
			return this->G;
		}

		HashNode *getNext()
		{
			return this->next;
		}

		void setNext(HashNode *next)
		{
			this->next = next;
		}

};
#endif
