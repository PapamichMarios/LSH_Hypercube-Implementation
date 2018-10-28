#ifndef __HYPER_NODE_H__
#define __HYPER_NODE_H__

template <typename K>
class HyperNode
{
	private:
		K point;
		std::string id;
		HyperNode* next;

	public:
		HyperNode(const K &point, const std::string id) : point(point), id(id), next(NULL) { }

		~HyperNode()
		{
			
		}
		
		K getKey()
		{
			return this->point;
		}

		std::string getId()
		{
			return this->id;
		}

		HyperNode *getNext()
		{
			return this->next;
		}

		void setNext(HyperNode *next)
		{
			this->next = next;
		}
		
};
#endif

