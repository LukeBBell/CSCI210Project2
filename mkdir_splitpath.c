
#include <stdbool.h>
#include "types.h"


extern struct NODE* root;
extern struct NODE* cwd;


void findNode (char* dirName,struct NODE* currentNode) {
	char* currdurr = malloc(256);
	char* dircpy = malloc(256);
	strcpy(dircpy,dirName);
	if (strcmp(dirName,"/") == 0) {
		currentNode = root;
		return;
	} else if (dirName[0] == '/') {
		currentNode = root;
		currdurr = strtok(dircpy+1,"/");
	} else {
		currentNode = cwd;
		currdurr = strtok(dircpy,"/");
	}
	while(currdurr != NULL) {
		currentNode = currentNode->childPtr;
		if (currdurr == NULL) {
			return;
		}
		while(strcmp(currentNode->name,currdurr)) {
			if(currentNode == NULL) {
				return;
			}
			currentNode = currentNode->siblingPtr;
		}
		currdurr = strtok(NULL,"/");
	}
	return;
}

//make directory
void mkdir(char pathName[]) {
	char* baseName = malloc(256);
	char* dirName = malloc(256);
	if (pathName == NULL || strlen(pathName) == 0) {
		pathName[0] = '/';
		printf("MKDIR ERROR: no path provided");
		
		return;
	}
	struct NODE* temp = malloc(sizeof(struct NODE));
	temp = splitPath(pathName,baseName,dirName);
	if (temp == NULL) {
		printf("ERROR: directory %s does not exist", strtok(dirName, '/'));
		return;
	}
	char* temp2 = strcat("/",baseName);
	struct NODE* child = malloc(sizeof(struct NODE));
	while (child != NULL) {
		if (strcmp(child->name,baseName) == 0) {
			printf("MKDIR ERROR: directory %s already exists\n",pathName);
			return;
		}
		child = child->siblingPtr;
	}
	struct NODE* newDir = (struct NODE*)malloc(sizeof(struct NODE));
	strncpy(newDir->name,baseName,63);
	newDir->name[63] = '/0';
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
	}	
	int offset = dirEnd - pathName;
	if (offset == 0) {
		strcpy(dirName,"/");
		strcpy(baseName,dirEnd+1);
	}else {
		if (dirEnd != NULL) {
			strncpy(dirName,pathName,offset);
			dirName[offset] = '/0';
			strcpy(baseName,dirEnd + 1);
		}
	}
	struct Node* parent = malloc(sizeof(struct NODE));
	findNode(dirName,parent);
    return parent;
}
