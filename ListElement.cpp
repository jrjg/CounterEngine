#include "TopInclude.h"
#include "List.h"

#include "ListElement.h"

template <class ObjectType> ListElement<ObjectType>::~ListElement()
{
	mpList->setLength(mpList->getLength() - 1);

	if (mpList->getLength() == 1) {
		mpList->setFirst(NULL);
		mpList->setLast(NULL);
		return;
	}
	if (mpList->getFirst()->mID == mID) {
		mpNext ? mpNext->setPrevious(NULL) : 1;
		mpList->setFirst(mpNext);
		return;
	}
	if (mpList->getLast()->mID == mID) {
		mpPrevious ? mpPrevious->setNext(NULL) : 1;
		mpList->setLast(mpPrevious);
		return;
	}
	mpNext->setPrevious(mpPrevious);
	mpPrevious->setNext(mpNext);

	(mDeleteContent) ? delete mpObject : 1;
	return;
}