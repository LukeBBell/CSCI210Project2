
#include <stdbool.h>
#include "types.h"


extern struct NODE* root;
extern struct NODE* cwd;


struct NODE* findNode (char* dirName) {
	struct NODE* currentNode = malloc(sizeof(struct NODE));
	char* currdurr = malloc(256);
	char* dircpy = malloc(256);
	strcpy(dircpy,dirName);
	if (strcmp(dirName,"/") == 0){
		return root;
	} else if (dirName[0] == '/') {
		currentNode = root;
		currdurr = strtok(dircpy+1,"/");
	} else {
		currentNode = cwd;
		currdurr = strtok(dircpy,"/");
	}
	while(currdurr != NULL) {
		currentNode = currentNode->childPtr;
		if (currentNode == NULL) {
			return currentNode;
		}
		while(strcmp(currentNode->name,currdurr) != 0) {
			if(currentNode->siblingPtr == NULL) {
				return NULL;
			}
			currentNode = currentNode->siblingPtr;
		}
		currdurr = strtok(NULL,"/");
	}

	return currentNode;
}

//make directory
void mkdir(char pathName[]) {
	char* baseName = malloc(256);
	char* dirName = malloc(256);
	if (strcmp(pathName,"/") == 0) {
		printf("MKDIR ERROR: no path provided\n");
		return;
	}
	struct NODE* temp = splitPath(pathName,baseName,dirName);
	if (temp == NULL) {
		printf("ERROR: directory %s does not exist\n", dirName);
		return;
	}
	struct NODE* child = temp->childPtr;
	while (child != NULL) {
		if (strcmp(child->name,baseName) == 0) {
			printf("MKDIR ERROR: directory %s already exists\n",pathName);
			return;
		}
		child = child->siblingPtr;
	}
	struct NODE* newDir = malloc(sizeof(struct NODE));
	strcpy(newDir->name,baseName);
	newDir->fileType = 'D';
	newDir->childPtr = NULL;
	newDir->siblingPtr = NULL;
	newDir->parentPtr = temp;
	if (temp->childPtr == NULL) {
		temp->childPtr = newDir;
	} else {
		struct NODE* child = temp->childPtr;
		while (child->siblingPtr != NULL) {
			child = child->siblingPtr;
		}
		child->siblingPtr = newDir;
	}

	
    printf("MKDIR SUCCESS: node %s successfully created\n",pathName);

    return;
}

//handles tokenizing and absolute/relative pathing options
struct NODE* splitPath(char* pathName, char* baseName, char* dirName){
	char* dirEnd = strrchr(pathName, '/');
	if (dirEnd == NULL) {
		dirName = malloc(256);
		strcpy(baseName,pathName);
	}else {	
	int offset = dirEnd - pathName;
		if (offset == 0) {
			strcpy(dirName,"/");
		}else {
			strncpy(dirName,pathName,offset);
			dirName[offset] = '\0';
		}
		strcpy(baseName,dirEnd + 1);
	}
	struct NODE* parent = findNode(dirName);
    return parent;
}
