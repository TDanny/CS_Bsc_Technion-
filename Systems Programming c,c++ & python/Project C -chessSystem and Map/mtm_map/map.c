#include "./mtm_map/map.h"
#include <stdio.h>
#include <stdlib.h>


typedef struct Element_t
{
    MapDataElement data;
    MapKeyElement key;
    struct Element_t* next;
    
}*Element;

struct Map_t    
{
    Element head;
    MapKeyElement iterator;
    copyMapDataElements copyDataFunction;
    copyMapKeyElements copyKeyFunction;
    freeMapDataElements freeDataFunction;
    freeMapKeyElements freeKeyFunction;
    compareMapKeyElements compareKeyElementsFunction;
};
static Element getElementByKey(Map map, MapKeyElement key);

/**
* mapCreate: Allocates a new empty map.
*
* @param copyDataElement - Function pointer to be used for copying data elements into
*  	the map or when copying the map.
* @param copyKeyElement - Function pointer to be used for copying key elements into
*  	the map or when copying the map.
* @param freeDataElement - Function pointer to be used for removing data elements from
* 		the map
* @param freeKeyElement - Function pointer to be used for removing key elements from
* 		the map
* @param compareKeyElements - Function pointer to be used for comparing key elements
* 		inside the map. Used to check if new elements already exist in the map.
* @return
* 	NULL - if one of the parameters is NULL or allocations failed.
* 	A new Map in case of success.
*/
Map mapCreate(copyMapDataElements copyDataElement, copyMapKeyElements copyKeyElement, freeMapDataElements freeDataElement,
              freeMapKeyElements freeKeyElement, compareMapKeyElements compareKeyElements)
              {
                //first lets check the input of the user
                if(!copyDataElement || !copyKeyElement || !freeDataElement || !freeKeyElement || !compareKeyElements)
                {
                    return NULL;
                }
                 Map newMap = malloc(sizeof(*newMap));
                 if(newMap==NULL)
                 {
                    return NULL;
                 }
                 newMap->head=NULL;
                 newMap->iterator=NULL;
                 newMap->copyDataFunction=copyDataElement;
                 newMap->copyKeyFunction=copyKeyElement;
                 newMap->freeDataFunction=freeDataElement;
                 newMap->freeKeyFunction=freeKeyElement;
                 newMap->compareKeyElementsFunction=compareKeyElements;

                 return newMap;
              }

/**
* mapDestroy: Deallocates an existing map. Clears all elements by using the
* stored free functions.
*
* @param map - Target map to be deallocated. If map is NULL nothing will be
* 		done
*/
void mapDestroy(Map map)
{
    if(!map)
    {
        return;
    }
    mapClear(map);
    free(map);
}

/**
* mapCopy: Creates a copy of target map.
* Iterator values for both maps is undefined after this operation.
*
* @param map - Target map.
* @return
* 	NULL if a NULL was sent or a memory allocation failed.
* 	A Map containing the same elements as map otherwise.
*/
Map mapCopy(Map map)
{
    if(!map)
    {
        return NULL;
    }
    Map newMap = mapCreate(map->copyDataFunction , map->copyKeyFunction , map->freeDataFunction , map->freeKeyFunction , map->compareKeyElementsFunction);
    if(!newMap)
    {
        return NULL;
    }
    map->iterator = NULL;
    Element oldMap = map->head;
    if(!oldMap)
    {
        return newMap;
    }
    Element cpyElement = malloc(sizeof(*cpyElement)); //first Element which head points to 
    if(!cpyElement)
    {
        free(map);
        return NULL;
    }
    newMap->head = cpyElement;
    cpyElement->key = map->copyKeyFunction(oldMap->key);
    cpyElement->data = map->copyDataFunction(oldMap->data);
    cpyElement->next = NULL;
    Element lastNext = cpyElement;
    int copyNumber;
    for(copyNumber=1 ; copyNumber<mapGetSize(map) ; copyNumber++)
    {
        oldMap = oldMap->next;
        cpyElement = malloc(sizeof(*cpyElement));
        if(!cpyElement)
        {
            mapDestroy(newMap);
            return NULL;
        }
        lastNext->next = cpyElement;
        cpyElement->key = map->copyKeyFunction(oldMap->key);
        cpyElement->data = map->copyDataFunction(oldMap->data);
        cpyElement->next = NULL;
    }
    return newMap;
}


/**
* mapGetSize: Returns the number of elements in a map
* @param map - The map which size is requested
* @return               
* 	-1 if a NULL pointer was sent.
* 	Otherwise the number of elements in the map.
*/
int mapGetSize(Map map)
{
    if(!map)
    {
        return -1;
    }
    else
    {
        int counter=0;
        Element toCount = map->head;
        while(toCount)
        {
            counter++;
            toCount = toCount->next;  
        }
    return counter;
    }
}


/**
* mapContains: Checks if a key element exists in the map. The key element will be
* considered in the map if one of the key elements in the map it determined equal
* using the comparison function used to initialize the map.
*
* @param map - The map to search in
* @param element - The element to look for. Will be compared using the
* 		comparison function.
* @return
* 	false - if one or more of the inputs is null, or if the key element was not found.
* 	true - if the key element was found in the map.
*/
bool mapContains(Map map, MapKeyElement element)
{
    if(!element || !map->head || !map)
    {
        return false;   
    }
    Element toCompere = map->head;
    while(toCompere)
    {
        if((map->compareKeyElementsFunction)(toCompere->key , element)==0)
        {
            return true;
        }
        toCompere = toCompere->next;

    }
    return false;
    
}



/**
*	mapPut: Gives a specified key a specific value.
*  Iterator's value is undefined after this operation.
*
* @param map - The map for which to reassign the data element
* @param keyElement - The key element which need to be reassigned
* @param dataElement - The new data element to associate with the given key.
*      A copy of the element will be inserted as supplied by the copying function
*      which is given at initialization and old data memory would be
*      deleted using the free function given at initialization.
* @return
* 	MAP_NULL_ARGUMENT if a NULL was sent as map
* 	MAP_OUT_OF_MEMORY if an allocation failed (Meaning the function for copying
* 	an element failed)
* 	MAP_SUCCESS the paired elements had been inserted successfully
*/
MapResult mapPut(Map map, MapKeyElement keyElement, MapDataElement dataElement)
{
    map->iterator = NULL;
    if(!map || !keyElement || !dataElement)
    {
        return MAP_NULL_ARGUMENT;
    }
    if(!mapContains(map ,keyElement))
    {
        Element newPut = malloc(sizeof(*newPut));
        if(!newPut)
        {
            return MAP_OUT_OF_MEMORY;
        }
        newPut->key = map->copyKeyFunction(keyElement);
        newPut->data = map->copyDataFunction(dataElement);
        Element currentElement = map->head;
        Element oneBeforeCurrentElement = NULL;
        if(!currentElement)  //in case this is the first element to enter the map
        {
            map->head = newPut;
            newPut->next = NULL;
            return MAP_SUCCESS;
        }
        
        while(currentElement != NULL && (map->compareKeyElementsFunction)(currentElement->key ,newPut->key)<0) //case of existence of elements
        {
            oneBeforeCurrentElement = currentElement;
            currentElement = currentElement->next;
        }
        if(!oneBeforeCurrentElement)//need to put the in head
        {
            map->head = newPut;
        }
        else
        {
            oneBeforeCurrentElement->next = newPut;
        }
        newPut->next = currentElement;
        return MAP_SUCCESS;
    }
    // in case there is a element with the same key the user provided us
    Element ptrElement = getElementByKey(map,keyElement);
    
    if(ptrElement->data!=NULL)
    {
        map->freeDataFunction(ptrElement->data);
    }

    ptrElement->data = map->copyDataFunction(dataElement);
    if(!ptrElement->data)
    {
        return MAP_OUT_OF_MEMORY;
    }
    return MAP_SUCCESS;

}


/**
*	mapGet: Returns the data associated with a specific key in the map.
*			Iterator status unchanged
*
* @param map - The map for which to get the data element from.
* @param keyElement - The key element which need to be found and whos data
we want to get.
* @return
*  NULL if a NULL pointer was sent or if the map does not contain the requested key.
* 	The data element associated with the key otherwise.
*/
MapDataElement mapGet(Map map, MapKeyElement keyElement)
{
    if(!map || !keyElement)
    {
        return NULL;
    }
    if(!mapContains(map ,keyElement))
    {
        return NULL;
    }
    Element ptrElement=getElementByKey(map,keyElement);
    if(!ptrElement || !ptrElement->data)
    {
        return NULL;
    }
    
    return ptrElement->data;
}


/**
* 	mapRemove: Removes a pair of key and data elements from the map. The elements
*  are found using the comparison function given at initialization. Once found,
*  the elements are removed and deallocated using the free functions
*  supplied at initialization.
*  Iterator's value is undefined after this operation.
*
* @param map -
* 	The map to remove the elements from.
* @param keyElement
* 	The key element to find and remove from the map. The element will be freed using the
* 	free function given at initialization. The data element associated with this key
*  will also be freed using the free function given at initialization.
* @return
* 	MAP_NULL_ARGUMENT if a NULL was sent to the function
*  MAP_ITEM_DOES_NOT_EXIST if an equal key item does not already exists in the map
* 	MAP_SUCCESS the paired elements had been removed successfully
*/
MapResult mapRemove(Map map, MapKeyElement keyElement)
{
    map->iterator = NULL;
    if(!keyElement || !map)
    {
        return MAP_NULL_ARGUMENT;
    }
    if(!mapContains(map, keyElement))
    {
        return MAP_ITEM_DOES_NOT_EXIST;
    }
    Element ptrData = map->head;
    Element oneBeforeRemovedElement = NULL;
    
    while((map->compareKeyElementsFunction)(ptrData->key ,keyElement)!=0 )
    {       
        oneBeforeRemovedElement = ptrData;
        ptrData=ptrData->next;
    }
    map->freeDataFunction(ptrData->data);
    map->freeKeyFunction(ptrData->key);
    Element newNext = ptrData->next;
    free(ptrData);
    if(!oneBeforeRemovedElement) // remove the head
    {
        map->head = newNext;
    }
    else
    {
        oneBeforeRemovedElement->next = newNext;
    }
    return MAP_SUCCESS;

}


/**
*	mapGetFirst: Sets the internal iterator (also called current key element) to
*	the first key element in the map. There doesn't need to be an internal order
*  of the keys so the "first" key element is any key element.
*	Use this to start iterating over the map.
*	To continue iteration use mapGetNext
*
* @param map - The map for which to set the iterator and return the first
* 		key element.
* @return
* 	NULL if a NULL pointer was sent or the map is empty.
* 	The first key element of the map otherwise
*/
MapKeyElement mapGetFirst(Map map)
{
    if(!map)
    {
        return NULL;
    }
    Element ptrFirstElement = map->head;
    if(!ptrFirstElement)
    {
        return NULL;
    }
    
    map->iterator = ptrFirstElement->key;
    return map->copyKeyFunction(map->iterator);
}



/**
*	mapGetNext: Advances the map iterator to the next key element and returns it.
*	The next key element is any key element not previously returned by the iterator.
* @param map - The map for which to advance the iterator
* @return
* 	NULL if reached the end of the map, or the iterator is at an invalid state
* 	or a NULL sent as argument
* 	The next key element on the map in case of success
*/
MapKeyElement mapGetNext(Map map)
{
    if(!map)
    {
        return NULL;
    }
    if(!map->iterator)
    {
        return NULL;
    }
    //the next 3 lines also deals with case which the next iterator will be NULL (end of the list)
    Element ptrNextElement =getElementByKey(map , map->iterator);
    if(!ptrNextElement->next)
    {
        map->iterator = NULL;
        return NULL;
    }
    map->iterator = (ptrNextElement->next)->key;
    return map->copyKeyFunction(map->iterator);
    
}



/**
* mapClear: Removes all key and data elements from target map.
* The elements are deallocated using the stored free functions.
* @param map
* 	Target map to remove all element from.
* @return
* 	MAP_NULL_ARGUMENT - if a NULL pointer was sent.
* 	MAP_SUCCESS - Otherwise.
*/
MapResult mapClear(Map map)
{
    if(!map)
    {
        return MAP_NULL_ARGUMENT;
    }
    while(map->head)
    {
        Element toClear = map->head;
        map->head=toClear->next;
        map->freeKeyFunction(toClear->key);
        map->freeDataFunction(toClear->data);
        toClear->next = NULL;
        free(toClear);
    }
     return MAP_SUCCESS;
}


/**
* getElementByKey: Removes all key and data elements from target map.
* The elements are deallocated using the stored free functions.
* @param map Target map to remove all element from.
* @param key and key of the element you want the data from
* @return
*   a pointer to data of specific element.
* 	NULL - if not find element my specif key or map is empty
*   
*/

static Element getElementByKey(Map map, MapKeyElement key)
{
    Element ptrElement = map->head;
    if(!ptrElement)
    {
        return NULL;
    }
    while(map->compareKeyElementsFunction(ptrElement->key,key)!=0 && ptrElement!=NULL)
    {
        ptrElement = ptrElement->next;
    }
    return ptrElement;

}