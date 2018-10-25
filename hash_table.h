#ifndef __HASH_TABLE_H__
#define __HASH_TABLE_H__

#include <string>

#include "hash_node.h"
#include "fi.h"
#include "g.h"

template <typename K>
class HashTable
{
	protected:
    	HashNode<K> **table;
		int tableSize;

	public:
		HashTable(int tableSize, int k, int dim)
		{
			/*== constructing hash table*/
			this->table = new HashNode<K> *[tableSize]();

			this->tableSize = tableSize;
		}
	
		~HashTable()
		{
			/*== destroying all buckets*/
			HashNode<K> * temp = NULL;
			HashNode<K> * prev = NULL;

			for(int i=0; i<this->tableSize; i++)
			{
				temp = table[i];
				while(temp != NULL)
				{
					prev = temp;
					temp = temp->getNext();
					delete prev;
				}

				table[i] = NULL;
			}

			delete[] table;
		}
		
    	virtual void put(const K &key) =0;
    
//	bool get(const K &key, V &value) {
  //      unsigned long hashValue = hashFunc(key);
   //     HashNode<K, V> *entry = table[hashValue];
//
 //       while (entry != NULL) {
  //          if (entry->getKey() == key) {
   //             value = entry->getValue();
    //            return true;
     //       }
      //      entry = entry->getNext();
      //  }
     //   return false;
   // }
};

template <typename K>
class HashTable_EUC : public HashTable<K>
{
	private:
		fi * hash_function;

	public:	
		HashTable_EUC(int tableSize, int k, int dim) : HashTable<K>(tableSize, k, dim)
		{
			/*== construct fi object, used for hashing*/
			this->hash_function = new fi(k, dim);
		}

		~HashTable_EUC()
		{
			delete[] hash_function;
		}

    	void put(const K &key)
		{
        	int hash_val = hash_function->hashValue(key, this->tableSize);
        	HashNode<K> *prev = NULL;
        	HashNode<K> *entry = this->table[hash_val];
			std::string G;

        	while (entry != NULL && entry->getKey() != key) 
			{
            	prev = entry;
            	entry = entry->getNext();
        	}

        	if (entry == NULL) 
			{
				G = hash_function->computeG(key);
            	entry = new HashNode<K>(key, G);

            	if (prev == NULL) 
				{
                	/*== insert as first bucket*/
                	this->table[hash_val] = entry;
            	} 	
				else 
				{
               		prev->setNext(entry);
            	}
        	} 
    	}

};

template <typename K>
class HashTable_COS : public HashTable<K>
{
	private:
		G * hash_function;

	public:
		HashTable_COS(int tableSize, int k, int dim) : HashTable<K>(tableSize, k, dim)
		{
			/*== construct g object, used for hashing*/
			this->hash_function = new G(k, dim);
		}

		~HashTable_COS()
		{
			delete[] hash_function;
		}

    	void put(const K &key)
		{
        	int hash_val = hash_function->hashValue(key);
        	HashNode<K> *prev = NULL;
        	HashNode<K> *entry = this->table[hash_val];

        	while (entry != NULL && entry->getKey() != key) 
			{
            	prev = entry;
            	entry = entry->getNext();
        	}

        	if (entry == NULL) 
			{
            	entry = new HashNode<K>(key, std::to_string(hash_val));

            	if (prev == NULL) 
				{
                	/*== insert as first bucket*/
                	this->table[hash_val] = entry;
            	} 	
				else 
				{
               		prev->setNext(entry);
            	}
        	} 
			
		}
		
};

#endif
