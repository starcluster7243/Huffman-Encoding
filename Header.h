#pragma once
#include <iostream>
#include <queue>
#include <map>
#include <climits> // for CHAR_BIT
#include <iterator>
#include <algorithm>

const int UniqueSymbols = 1 << CHAR_BIT;
const char* SampleString = "Abstract People have observed the sky since thousands of years.The oldest observations of variable stars date back some 3200 years.An ancient Egyptian calendar of lucky and unlucky days is considered to be the first proof of observation of a variable star.It was the eclipsing binary, we will explain later what exactly this is, Algol in the constellation Perseus.The period of the star system is about 2.87 days so it is very comfortable for naked eye observation.During the years till then humans evolved and developed instruments for much more accurate and detailed observations of the stars in our Galaxy and even in galaxies far far away. The telescope was invented at the beginning of the 17th century in the Netherlands and was later developed by Galileo.In the past hundred years we managed to place our eyes, through our creations outside the Earth`s atmosphere to look into the cosmic void, searching for answers about our Universe.In this paper we will discussed the essence of variable stars.What exactly are they ? Why are they so important and what information we can gain by observing them ? We will show our new method for searching for variable stars developed by us.And during our research we made observations from the astronomical site located at Beli Brezi, near Kurdjali, Bulgaria.In our expedition we made photographic observations of the open cluster NGC 7243 for several nights.In the image processing afterwards we have determined 8 possible variable stars in the cluster.For reference in the international catalog there are only 2 known and confirmed variables in the area of NGC 7243. So we may have found at least 5 or 6 new variable stars previously being unknown to humanity.Of course there is a lot of work to be done from now on, but this paper is concentrated to our preliminary research and the results from it. Keywords : Variable star, astronomy, new method, NGC 7243, amateur observations";

typedef std::vector<bool> HuffCode;
typedef std::map<char, HuffCode> HuffCodeMap;

class INode
{
public:
	const int f;

	virtual ~INode() {}

protected:
	INode(int f) : f(f) {}
};

class InternalNode : public INode
{
public:
	INode *const left;
	INode *const right;

	InternalNode(INode* c0, INode* c1) : INode(c0->f + c1->f), left(c0), right(c1) {}
	~InternalNode()
	{
		delete left;
		delete right;
	}
};

class LeafNode : public INode
{
public:
	const char c;

	LeafNode(int f, char c) : INode(f), c(c) {}
};

struct NodeCmp
{
	bool operator()(const INode* lhs, const INode* rhs) const { return lhs->f > rhs->f; }
};

INode* BuildTree(const int(&frequencies)[UniqueSymbols])
{
	std::priority_queue<INode*, std::vector<INode*>, NodeCmp> trees;

	for (int i = 0; i < UniqueSymbols; ++i)
	{
		if (frequencies[i] != 0)
			trees.push(new LeafNode(frequencies[i], (char)i));
	}
	while (trees.size() > 1)
	{
		INode* childR = trees.top();
		trees.pop();

		INode* childL = trees.top();
		trees.pop();

		INode* parent = new InternalNode(childR, childL);
		trees.push(parent);
	}
	return trees.top();
}

void GenerateCodes(const INode* node, const HuffCode& prefix, HuffCodeMap& outCodes)
{
	if (const LeafNode* lf = dynamic_cast<const LeafNode*>(node))
	{
		outCodes[lf->c] = prefix;
	}
	else if (const InternalNode* in = dynamic_cast<const InternalNode*>(node))
	{
		HuffCode leftPrefix = prefix;
		leftPrefix.push_back(false);
		GenerateCodes(in->left, leftPrefix, outCodes);

		HuffCode rightPrefix = prefix;
		rightPrefix.push_back(true);
		GenerateCodes(in->right, rightPrefix, outCodes);
	}
}

