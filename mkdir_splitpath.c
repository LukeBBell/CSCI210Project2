
#include <stdbool.h>
#include "types.h"


extern struct NODE* root;
extern struct NODE* cwd;

struct NODE* findNode(struct NODE* parent, char* targetPath,char* targetName) {
	struct NODE* f = parent;
	while (f != NULL || !(strcmp(f->name,targetName))){
		if (strcmp(parent->name,targetName)) {
			f = parent;
		}else {
			targetPath = strchr(targetPath,'/') + 1;
			char* hunt = NULL;
			char* target = NULL;
			bool end = false;
			char* tarEnd = strchr(targetPath,'/')-1;
			if (tarEnd == NULL) {
				end = true;
			}	
			int offset = tarEnd - targetPath;
			strcpy(target,tarEnd +1);
			strncpy(hunt,targetPath,offset);
			bool found = false;
			while (!found) {
				if (parent->siblingPtr == NULL) {
					found = true;
				} else if(strcmp(parent->siblingPtr->name,targetName)) {
					f = parent->siblingPtr;
					found = true;
				} else if (end != true) {
					if (strcmp(parent->siblingPtr->name,hunt)) {
						parent = parent->siblingPtr;
						found = true;
					}	
				} else {
					f = findNode(parent->siblingPtr,target,targetName);
				}
			}
			if (end != true) {
				if (parent->childPtr == NULL) {
					f = NULL;
				} else {
					f = findNode(parent->childPtr,target,targetName);
				}
			}	
		}
	}
	return f;
}

//make directory
void mkdir(char pathName[]) {
	char* baseName = NULL;
	char* dirName = NULL;
	if (pathName == NULL || strlen(pathName) == 0) {
		pathName[0] = '/';
		printf("MKDIR ERROR: no path provided");
		return;
	}
	struct NODE* temp = NULL;
	temp = splitPath(pathName,baseName,dirName);
	if (temp == NULL) {
		printf("ERROR: directory %s does not exist", strtok(dirName, "/"));
		return;
	}
	char* temp2 = strcat("/",baseName);
	if (findNode(temp,temp2,baseName) != NULL) {
		printf("ERROR: directory %s already exists", baseName);
		return;
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
		dirName = "";
		baseName = pathName;
		return NULL;
	}	
	int offset = dirEnd - pathName;
	if (offset == 0) {
		dirName = "/";
		baseName = "";
		return NULL;
	}
	strncpy(dirName,pathName,offset);
	dirName[offset] = '/0';
	strcpy(baseName,dirEnd + 1);
	struct Node* parent = NULL;
	char* parentName = NULL;
	strcpy(parentName,strrchr(dirName, '/')+1);
	if (pathName[0] == '/') {
		parent = root;
	} else {
		parent = findNode(root,dirName,parentName);
	}
    return parent;
}
