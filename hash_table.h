#ifndef __HASH_TABLE_H__
#define __HASH_TABLE_H__

#include <string>
#include <algorithm>
#include <fstream>
#include <climits>
#include <ctime>

#include "hash_node.h"
#include "help_functions.h"
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
		
    	virtual void put(const K &key, std::string identifier) =0;
		virtual void ANN(const K &query, std::ofstream& outputfile) =0;
		virtual void NN (const K &query, std::ofstream& outputfile) =0;
		virtual void RS (const K &query,std::ofstream& outputfile, int c, int R) =0;
};

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

    	void put(const K &key, std::string identifier)
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
            	entry = new HashNode<K>(key, G, identifier);

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

		/*== Neighbour functions*/
		void NN(const K &query, std::ofstream& outputfile)
		{
			double distance;
			HashNode<K> * temp = NULL;
			clock_t end_time;

			double min_distance = INT_MAX;
			std::string identifier = "NONE";

			std::string G = hash_function->computeG(query);
			const clock_t begin_time = clock();

			/*== start iterating through the hash table*/
			for(int i=0; i<this->tableSize; i++)
			{
				/*== assign temp to the head of the bucket*/
				temp = this->table[i];	
				
				/*== iterate through every node in the bucket*/
				while( temp != NULL )
				{
					/*== compare the g(query) with the g(point)*/
					if( G != temp->getG() )
					{
						temp = temp->getNext();
						continue;
					}

					/*== if they match calculate distance*/
					distance = help_functions::euclidean_distance(query, temp->getKey());
					
					if(distance < min_distance)
					{
						min_distance = distance;
						identifier = temp->getId();
					}

					/*== continue the iteration*/
					temp = temp->getNext();
				}
			}
			
			end_time = clock();

			/*== print to file*/
			outputfile << identifier << std::endl;
			outputfile << "distanceTrue: " << min_distance << std::endl;
			outputfile << "tTrue: " << float(end_time - begin_time) / CLOCKS_PER_SEC << " secs" << std::endl;
			
		}

		void ANN(const K &query, std::ofstream& outputfile)
		{
			std::vector<double> distance_list;
			std::vector<std::string> identifier_list;
			double distance=0;
			clock_t end_time = clock();

			int hash_val		= hash_function->hashValue(query, this->tableSize);
			std::string G		= hash_function->computeG(query);
			HashNode<K> * temp  = this->table[hash_val];
			const clock_t begin_time = clock();


			/*== get every point saved on the bucket and find the euclidean distance with query point*/
			while( temp != NULL )
			{
				/*== compare the g(query) with the g(point)*/
				if( G != temp->getG() )
				{
					temp = temp->getNext();
					continue;
				}


				/*== if they match, save the distance*/
				distance = help_functions::euclidean_distance(query, temp->getKey());
	
				distance_list.push_back(distance);
				identifier_list.push_back(temp->getId());

				/*== continue the iteration*/
				temp = temp->getNext();
			}
			end_time = clock();

			/*== print to file*/
			if( distance_list.size() > 0 )
			{
				auto min = std::min_element(distance_list.begin(), distance_list.end());
				int min_index = std::distance(distance_list.begin(), min);

				outputfile << identifier_list[min_index] << std::endl;
				outputfile << "distanceLSH: " << *min << std::endl;
				outputfile << "tLSH: " << float(end_time - begin_time) / CLOCKS_PER_SEC << " secs" << std::endl;
			}
			else
			{
				outputfile << "-" << std::endl;
				outputfile << "distanceLSH: -" << std::endl;
				outputfile << "tLSH: " << std::endl;
			}

		}

		void RS(const K &query, std::ofstream& outputfile, int c, int R)
		{
			std::vector<double> distance_list;
			std::vector<std::string> identifier_list;
			double distance=0;

			int hash_val		= hash_function->hashValue(query, this->tableSize);
			std::string G		= hash_function->computeG(query);
			HashNode<K> * temp  = this->table[hash_val];

			/*== iterate through the bucket*/
			while( temp != NULL )
			{
				/*== compare the g(query) with the g(point)*/
				if( G != temp->getG() )
				{
					temp = temp->getNext();
					continue;
				}
			
				/*== if they match calculate the distance*/
				distance = help_functions::euclidean_distance(query, temp->getKey());
				if(distance<c*R)
				{
					distance_list.push_back(distance);
					identifier_list.push_back(temp->getId());
				}	

				/*== iterate to the next node*/
				temp = temp->getNext();	
			}	

			/*== print neighbours*/
			for(unsigned int i=0; i<distance_list.size(); i++)
				outputfile << identifier_list[i] << " " << distance_list[i] << std::endl;
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

    	void put(const K &key, std::string identifier)
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
            	entry = new HashNode<K>(key, std::to_string(hash_val), identifier);

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

		/*== Neighbour functions*/
		void NN(const K &query, std::ofstream& outputfile)
		{
			double distance;
			HashNode<K> * temp = NULL;
			clock_t end_time;

			double min_distance = INT_MAX;
			std::string identifier = "NONE";
			const clock_t begin_time = clock();

			/*== start iterating through the hash table*/
			for(int i=0; i<this->tableSize; i++)
			{
				/*== assign temp to the head of the bucket*/
				temp = this->table[i];	
				
				/*== iterate through every node in the bucket*/
				while( temp != NULL )
				{
					/*== calculate distance*/
					distance = help_functions::euclidean_distance(query, temp->getKey());
					
					if(distance < min_distance)
					{
						min_distance = distance;
						identifier = temp->getId();
					}

					/*== continue the iteration*/
					temp = temp->getNext();
				}
			}
			end_time = clock();

			/*== print to file*/
			outputfile << identifier << std::endl;
			outputfile << "distanceTrue: " << min_distance << std::endl;
			outputfile << "tTrue: " << float(end_time - begin_time) / CLOCKS_PER_SEC << " secs" <<  std::endl;
			
		}

		void ANN(const K &query, std::ofstream& outputfile)
		{
			std::vector<double> distance_list;
			std::vector<std::string> identifier_list;
			double distance=0;
			clock_t end_time;

			int hash_val		= hash_function->hashValue(query);
			HashNode<K> * temp  = this->table[hash_val];
			const clock_t begin_time = clock();

			/*== get every point saved on the bucket and find the euclidean distance with query point*/
			while( temp != NULL )
			{
				/*== calculate the distance*/
				distance = help_functions::euclidean_distance(query, temp->getKey());
	
				distance_list.push_back(distance);
				identifier_list.push_back(temp->getId());

				/*== continue the iteration*/
				temp = temp->getNext();
			}
			end_time = clock();


			/*== print to file*/
			if( distance_list.size() > 0 )
			{
				auto min = std::min_element(distance_list.begin(), distance_list.end());
				int min_index = std::distance(distance_list.begin(), min);

				outputfile << identifier_list[min_index] << std::endl;
				outputfile << "distanceLSH: " << *min << std::endl;
				outputfile << "tLSH: " << float(end_time - begin_time) / CLOCKS_PER_SEC << " secs" << std::endl;
			}
			else
			{
				outputfile << "-" << std::endl;
				outputfile << "distanceLSH: -" << std::endl;
				outputfile << "tLSH: " << std::endl;
			}

		}

		void RS(const K &query, std::ofstream& outputfile, int c, int R)
		{
			std::vector<double> distance_list;
			std::vector<std::string> identifier_list;
			double distance=0;

			int hash_val		= hash_function->hashValue(query);
			HashNode<K> * temp  = this->table[hash_val];

			/*== iterate through the bucket*/
			while( temp != NULL )
			{
				/*== calculate the distance*/
				distance = help_functions::euclidean_distance(query, temp->getKey());
				if(distance<c*R)
				{
					distance_list.push_back(distance);
					identifier_list.push_back(temp->getId());
				}	

				/*== iterate to the next node*/
				temp = temp->getNext();	
			}	

			/*== print neighbours*/
			for(unsigned int i=0; i<distance_list.size(); i++)
				outputfile << identifier_list[i] << " " << distance_list[i] << std::endl;
		}
};

#endif
