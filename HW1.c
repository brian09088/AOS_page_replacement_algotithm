#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define REFERENCE_TIME 200000
#define PAGES_NUMBER_FLOOR 1
#define PAGES_NUMBER_CEILING 1000
#define RANDOM_WINDOW_FLOOR 1
#define RANDOM_WINDOW_CEILING 20
#define LOCALITY_WINDOW_FLOOR 1000
#define LOCALITY_WINDOW_CEILING 2500

typedef struct frame {
    int value;
    int referenced;
    int dirty;
    int info; // for ARB
    struct frame *next;
} Frame;

typedef struct frameList {
    int size;
    Frame *head;
    Frame *tail;
} FrameList;

FrameList *createFrameList() {
    FrameList *frameList = (FrameList*)malloc(sizeof(FrameList));
    frameList -> size = 0;
    frameList -> head = NULL;
    frameList -> tail = NULL;

    return frameList;
}

void processReferenceStringWithRandom(int *referenceString) {
    int currentIndex = 0;
    int isStillProcessing = 1;

    while(isStillProcessing) {
        int windowSize = rand() % (RANDOM_WINDOW_CEILING - RANDOM_WINDOW_FLOOR + 1) + RANDOM_WINDOW_FLOOR;
        int startNumber = rand() % (PAGES_NUMBER_CEILING - PAGES_NUMBER_FLOOR + 1) + PAGES_NUMBER_FLOOR;

        for(int i = 0; i < windowSize; i++) {
            referenceString[currentIndex] = startNumber;
            startNumber += 1;

            if(startNumber == PAGES_NUMBER_CEILING + 1) {
                startNumber = 1;
            }

            currentIndex += 1;
            if(currentIndex == REFERENCE_TIME) {
                isStillProcessing = 0;
                break;
            }
        }
    }
}

void processReferenceStringWithLocality(int *referenceString) {
    int currentIndex = 0;
    int innerWindowSize = 5;
    int pickedNumber = 0;
    int isStillProcessing = 1;

    while(isStillProcessing) {
        int windowSize = rand() % (LOCALITY_WINDOW_CEILING - LOCALITY_WINDOW_FLOOR + 1) + LOCALITY_WINDOW_FLOOR;
        int startNumber = rand() % (PAGES_NUMBER_CEILING - PAGES_NUMBER_FLOOR + 1) + PAGES_NUMBER_FLOOR;

        for(int i = 0; i < windowSize; i++) {
            // startNumber > (PAGES_NUMBER_CEILING - innerWindowSize + 1) || startNumber < (PAGES_NUMBER_FLOOR + innerWindowSize - 1)
            if(startNumber < (PAGES_NUMBER_FLOOR + innerWindowSize - 1)) {
                int temporaryPicked[2];
                int randomIndex = rand() % 2;

                temporaryPicked[0] = rand() % (PAGES_NUMBER_CEILING - startNumber + 1) + startNumber;
                temporaryPicked[1] = rand() % ((PAGES_NUMBER_FLOOR + innerWindowSize - (PAGES_NUMBER_CEILING - startNumber + 1) - 1) - PAGES_NUMBER_FLOOR + 1) + PAGES_NUMBER_FLOOR;

                pickedNumber = temporaryPicked[randomIndex];
	    } else {
                pickedNumber = rand() % (startNumber - (startNumber - innerWindowSize + 1) + 1) + (startNumber - innerWindowSize + 1);
            }

            referenceString[currentIndex] = pickedNumber;
            currentIndex += 1;

            if(currentIndex > REFERENCE_TIME) {
                isStillProcessing = 0;
                break;
            }
        }
    }
}

void processReferenceStringWithSequenceAndJump(int *referenceString) {
    int isStillProcessing = 1;
    int currentIndex = 0;

    // 跳躍數量設置 5 ~ 10
    int smallWindowFloor = 5;
    int smallWindowCeiling = 10;
    // 一般 references 數量設置 50 ~ 100
    int bigWindowFloor = 50;
    int bigWindowCeiling = 100;

    while(isStillProcessing) {
        // 跳躍的機率，random 1 ~ 10
        int jumpingProbability = rand() % 10 + 1;
        int windowSize = 0;
        int startNumber; // 會記錄上一次的 startNumber

        // 在此設有 80% 的機率會跳躍
        if(jumpingProbability > 2) {
            int isJumpingFar = 0;
            int temporaryStartNumber;
            int temporaryFloor;
            int temporaryCeiling;

            windowSize = rand() % (smallWindowCeiling - smallWindowFloor + 1) + smallWindowFloor;

            while(!isJumpingFar) {
                temporaryStartNumber = rand() % ((PAGES_NUMBER_CEILING - windowSize + 1) - PAGES_NUMBER_FLOOR + 1) + PAGES_NUMBER_FLOOR;
                temporaryFloor = temporaryStartNumber - 100;
                temporaryCeiling = temporaryStartNumber + 100;

                // 處理邊界
                if(temporaryFloor < PAGES_NUMBER_FLOOR || temporaryCeiling > PAGES_NUMBER_CEILING) {
                    continue;
                }

                // 如果在上一個 startNumber 的 +-100 內，重新產生，反之則設定新數字
                if(!((startNumber > temporaryStartNumber - 100) && (startNumber < temporaryStartNumber + 100))) {
                    startNumber = temporaryStartNumber;
                    isJumpingFar = 1;
                }
            }
        } else {
            windowSize = rand() % (bigWindowCeiling - bigWindowFloor + 1) + bigWindowFloor;
            startNumber = rand() % ((PAGES_NUMBER_CEILING - windowSize + 1) - PAGES_NUMBER_FLOOR + 1) + PAGES_NUMBER_FLOOR;
        }

        for(int i = 0; i < windowSize; i++) {
            // 產生的 reference 為等差數列 (n, n+1, n+2...)
            referenceString[currentIndex] = startNumber;
            startNumber += 1;

            currentIndex += 1;
            if(currentIndex == REFERENCE_TIME) {
                isStillProcessing = 0;
                break;
            }
        }
    }
}

void printReferenceString(int *referenceString) {
    for(int i = 0; i < REFERENCE_TIME; i++) {
        printf("%d ", referenceString[i]);
    }
}

// 檢查 reference string 的元素是否越界
void checkReferenceString(int *referenceString) {
    for(int i = 0; i < REFERENCE_TIME; i++) {
        if(referenceString[i] < PAGES_NUMBER_FLOOR || referenceString[i] > PAGES_NUMBER_CEILING) {
            printf("%d", referenceString[i]);
            printf("\nReference String is out-of-bounds.\n");
        }
    }
}

void printFrames(FrameList *frameList) {
    Frame *temporaryFrame = frameList -> head;

    while(temporaryFrame != NULL) {
        printf("%d ", temporaryFrame -> value);
        temporaryFrame = temporaryFrame -> next;
    }

    free(temporaryFrame);
    printf("\n");
}

void freeAllTheFrames(FrameList* frameList) {
    Frame *frame = frameList -> head;
    Frame *temporaryFrame;

    while(frame != NULL) {
        temporaryFrame = frame;
        frame = frame -> next;
        free(temporaryFrame);
    }

    free(frameList);
}
// FIFO 演算法
void pageReplacementWithFIFO(int *referenceString, int frameSize, int *result) {
    int pageFaultCount = 0;
    int interruptCount = 0;
    int diskWriteCount = 0;

    FrameList *frameList = createFrameList();

    for(int i = 0; i < REFERENCE_TIME; i++) {
        Frame *temporaryFrame = frameList -> head;
        int hit = 0;

        while(temporaryFrame != NULL) {
            // 當已經存在 frame 中 (hit)
            if(referenceString[i] == temporaryFrame -> value) {
                hit = 1;
                break;
            }
            temporaryFrame = temporaryFrame -> next;
        }

        // hit 不計算 page fault
        if(hit) {
            continue;
        }

        free(temporaryFrame);
        // 沒有在 frame 中 (miss)
        Frame *frame = (Frame*)malloc(sizeof(Frame));
        frame -> value = referenceString[i];
        frame -> next = NULL;

        // 設定 dirty bit（使用 50/50）
        int isDirty = rand() % 2;

        frame -> dirty = isDirty;

        // 處理第一個
        if(frameList -> head == NULL) {
            frameList -> head = frame;
            frameList -> size += 1;
        } else {
            // Frame *temporaryFrame = frameList -> head;
            frameList -> tail -> next = frame;

            // frame 仍有空間
            if(frameList -> size < frameSize) {
                frameList -> size += 1;
            // frame 無空間並進行替換
            } else {
                Frame *firstFrame = frameList -> head;
                frameList -> head = firstFrame -> next;
                free(firstFrame);

                // 處理 disk write
                if(frame -> dirty) {
                    frame -> dirty = 0;
                    diskWriteCount += 1;
                }
            }
        }

        frameList -> tail = frame;
        // 當放入 frame 或需要 replacement 時，page fault +1
        pageFaultCount += 1;
        // printFrames(frameList);
    }

    freeAllTheFrames(frameList);

    result[0] = pageFaultCount;
    result[1] = interruptCount;
    result[2] = diskWriteCount;
}
//Enhanced second-chance 演算法
void pageReplacementWithEnhanced(int *referenceString, int frameSize, int *result) {
    int pageFaultCount = 0;
    int interruptCount = 0;
    int diskWriteCount = 0;

    FrameList *frameList = createFrameList();

    for(int i = 0; i < REFERENCE_TIME; i++) {
        Frame *temporaryFrame = frameList -> head;
        int hit = 0;

        while(temporaryFrame != NULL) {
            // 當已經存在 frame 中 (hit)
            if(referenceString[i] == temporaryFrame -> value) {
                hit = 1;
                break;
            }
            temporaryFrame = temporaryFrame -> next;
        }

        // hit 不計算 page fault
        if(hit) {
            continue;
        }

        free(temporaryFrame);
        // 沒有在 frame 中 (miss)
        Frame *frame = (Frame*)malloc(sizeof(Frame));
        frame -> value = referenceString[i];
        frame -> next = NULL;

        // 設定 dirty bit（使用 50/50）
        int isDirty = rand() % 2;

        frame -> dirty = isDirty;
        // 處理第一個
        if(frameList -> head == NULL) {
            frame -> info = 0b00000000;
            frameList -> head = frame;
            frameList -> size += 1;
            frameList -> tail = frame;
        } else {
            // to be continued...
            // 遍歷frameList找到要被替換的頁面
            Frame *currentFrame = frameList -> head;
            Frame *victimFrame = NULL;

            while (currentFrame != NULL) {
                if (currentFrame -> referenced == 0 && currentFrame -> dirty == 0) {
                    // (0, 0) - 最近没有被使用，也没有被修改，選擇替換
                    victimFrame = currentFrame;
                    break;
                }
                currentFrame -> referenced = 0;  // 重置頁面為未使用狀態
                currentFrame = currentFrame -> next;
            }

            if (victimFrame == NULL) {
                currentFrame = frameList -> head;
                while (currentFrame != NULL) {
                    if (currentFrame -> referenced == 0 && currentFrame -> dirty == 1) {
                        // (0, 1) - 最近没有被使用，但有被修改，選擇替換
                        victimFrame = currentFrame;
                        break;
                    }
                    currentFrame = currentFrame -> next;
                }
            }

            if (victimFrame == NULL) {
                currentFrame = frameList -> head;
                while (currentFrame != NULL) {
                    if (currentFrame -> referenced == 1 && currentFrame -> dirty == 0) {
                        // (1, 0) - 最近有被使用，但没有被修改，選擇替換
                        victimFrame = currentFrame;
                        break;
                    }
                    currentFrame = currentFrame -> next;
                }
            }

            if (victimFrame == NULL) {
                currentFrame = frameList -> head;
                while (currentFrame != NULL) {
                    if (currentFrame -> referenced == 1 && currentFrame -> dirty == 1) {
                        // (1, 1) - 最近有被使用，而且也有被修改，選擇替換
                        victimFrame = currentFrame;
                        break;
                    }
                    currentFrame = currentFrame -> next;
                }
            }

            if (victimFrame != NULL) {
                // 處理 disk write
                if (victimFrame -> dirty) {
                    diskWriteCount += 1;
                }

                // 替换頁面
                if (victimFrame == frameList -> head) {
                    frameList -> head = frameList -> head -> next;
                } else {
                    currentFrame = frameList -> head;
                    while (currentFrame -> next != victimFrame) {
                        currentFrame = currentFrame -> next;
                    }
                    currentFrame -> next = victimFrame -> next;
                }

                free(victimFrame);
            }

            // 將新頁面添加到frameList中
            frame -> info = 0b00000000;
            frameList -> tail -> next = frame;
            frameList -> tail = frame;
            frameList -> size += 1;
        }
        pageFaultCount += 1;
    }
    result[0] = pageFaultCount;
    result[1] = interruptCount;
    result[2] = diskWriteCount;

    freeAllTheFrames(frameList);
}

//ARB演算法
void pageReplacementWithARB(int *referenceString, int frameSize, int *result) {
    // 二進位表示在 gcc 支援
    // https://gcc.gnu.org/onlinedocs/gcc/Binary-constants.html
    // 在此設定 8 bits
    // 最大為 11111111 = (decimal) 255

    int pageFaultCount = 0;
    int interruptCount = 0;
    int diskWriteCount = 0;

    FrameList *frameList = createFrameList();

    for(int i = 0; i < REFERENCE_TIME; i++) {
        Frame *temporaryFrame = frameList -> head;
        int hit = 0;

        while(temporaryFrame != NULL) {
            // 當已經存在 frame 中 (hit)
            if(referenceString[i] == temporaryFrame -> value) {
                // OS 進行動作，設定 info
                interruptCount += 1;

                // leftmost 設定 1
                temporaryFrame -> info += 128;

                hit = 1;
                break;
            }
            temporaryFrame = temporaryFrame -> next;
        }

        // hit 不計算 page fault
        if(hit) {
            continue;
        }

        // 每當 8 的倍數時，處理 shift
        // 向右 shift 1 位
        if(i % 8 == 0 && i != 0) {
            // OS 進行動作，進行 shift
            interruptCount += 1;

            Frame *currentFrame = frameList -> head;

            while(currentFrame != NULL) {
                currentFrame -> info >>= 1;
                currentFrame = currentFrame -> next;
            }

            free(currentFrame);
        }

        free(temporaryFrame);
        // 沒有在 frame 中 (miss)
        Frame *frame = (Frame*)malloc(sizeof(Frame));
        frame -> value = referenceString[i];
        frame -> next = NULL;

        // 設定 dirty bit（使用 50/50）
        int isDirty = rand() % 2;

        frame -> dirty = isDirty;

        // 處理第一個
        if(frameList -> head == NULL) {
            frame -> info = 0b00000000;
            frameList -> head = frame;
            frameList -> size += 1;
            frameList -> tail = frame;
        } else {
            // frame 仍有空間
            if(frameList -> size < frameSize) {
                frame -> info = 0b00000000;
                frameList -> size += 1;
                frameList -> tail -> next = frame;
                frameList -> tail = frame;
            // frame 無空間並進行替換
            } else {
                // 設定新替換者 leftmost 為 1
                frame -> info = 0b10000000;
		
                Frame *currentFrame = frameList -> head;
                Frame *targetFrame = currentFrame;
                int largestInfo = 0b11111111;
		// 抽換bits規則
                while(currentFrame != NULL) {
                    if(currentFrame -> info <= largestInfo) {
                        targetFrame = currentFrame;
                        largestInfo = currentFrame -> info;
                    }

                    currentFrame = currentFrame -> next;
                }

                // 處理選中是 head 時
                if(targetFrame == frameList -> head) {
                    frame -> next = targetFrame -> next;
                    frameList -> head = frame;
                // 處理選中是 tail 或中間時
                } else {
                    Frame *previousFrame = frameList -> head;
                    Frame *nextFrame;

                    while(previousFrame -> next != targetFrame) {
                        previousFrame = previousFrame -> next;
                    }

                    // 處理選中是 tail 時
                    if(previousFrame -> next -> next == NULL) {
                        previousFrame -> next = frame;
                        frameList -> tail = frame;
                    // 處理選中是中間時
                    } else {
                        targetFrame = previousFrame -> next;
                        nextFrame = targetFrame -> next;

                        previousFrame -> next = frame;
                        frame -> next = nextFrame;

                        free(targetFrame);
                    }
                }

                // 處理 disk write
                if(frame -> dirty) {
                    frame -> dirty = 0;
                    diskWriteCount += 1;
                }
            }
        }
        pageFaultCount += 1;
    }

    result[0] = pageFaultCount;
    result[1] = interruptCount;
    result[2] = diskWriteCount;

    freeAllTheFrames(frameList);
}
//自行設計演算法
void pageReplacementWithDistance(int *referenceString, int frameSize, int *result) {
    int pageFaultCount = 0;
    int interruptCount = 0;
    int diskWriteCount = 0;

    FrameList *frameList = createFrameList();

    for(int i = 0; i < REFERENCE_TIME; i++) {
        Frame *temporaryFrame = frameList -> head;
        int hit = 0;

        while(temporaryFrame != NULL) {
            // 當已經存在 frame 中 (hit)
            if(referenceString[i] == temporaryFrame -> value) {
                hit = 1;
                break;
            }
            temporaryFrame = temporaryFrame -> next;
        }

        // hit 不計算 page fault
        if(hit) {
            continue;
        }

        free(temporaryFrame);
        // 沒有在 frame 中 (miss)
        Frame *frame = (Frame*)malloc(sizeof(Frame));
        frame -> value = referenceString[i];
        frame -> next = NULL;

        // 設定 dirty bit（使用 50/50）
        int isDirty = rand() % 2;

        frame -> dirty = isDirty;

        // 處理第一個
        if(frameList -> head == NULL) {
            frameList -> head = frame;
            frameList -> size += 1;
            frameList -> tail = frame;
        } else {
            // frame 仍有空間
            if(frameList -> size < frameSize) {
                frameList -> size += 1;
                frameList -> tail -> next = frame;
                frameList -> tail = frame;
            // frame 無空間並進行替換
            } else {
                // OS 進行動作，計算距離
                interruptCount += 1;

                // 計算距離
                int longestDistance = PAGES_NUMBER_FLOOR;
                Frame *calculatingFrame = frameList -> head;
                Frame *longestDistanceFrame = frameList -> head;

                for(int j = 0; j < frameSize; j++) {
                    int clockWiseDistance = 0;
                    int counterClockWiseDistance = 0;
                    int currentFrameLongestDistance;


                    if(frame -> value > calculatingFrame -> value) {
                        clockWiseDistance = (PAGES_NUMBER_CEILING - frame -> value) + 1 +(calculatingFrame -> value - PAGES_NUMBER_FLOOR);
                        counterClockWiseDistance = frame -> value - calculatingFrame -> value;
                    } else {
                        clockWiseDistance = (PAGES_NUMBER_CEILING - calculatingFrame -> value) + 1 + (frame -> value - PAGES_NUMBER_FLOOR);
                        counterClockWiseDistance = calculatingFrame -> value - frame -> value;
                    }

                    // 順時鐘和逆時鐘距離，選擇較短的
                    if(clockWiseDistance < counterClockWiseDistance) {
                        currentFrameLongestDistance = clockWiseDistance;
                    } else {
                        currentFrameLongestDistance = counterClockWiseDistance;
                    }

                    // 比對目前最長距離
                    if(currentFrameLongestDistance > longestDistance) {
                        longestDistance = currentFrameLongestDistance;
                        longestDistanceFrame = calculatingFrame;
                    } else if(currentFrameLongestDistance == longestDistance) {
                        if(calculatingFrame -> value < longestDistanceFrame -> value) {
                            longestDistanceFrame = calculatingFrame;
                        }
                    }

                    calculatingFrame = calculatingFrame -> next;
                }

                // 代換距離最長的 frame
                // 處理 head
                if(longestDistanceFrame == frameList -> head) {
                    Frame *temporaryFrame = frameList -> head;

                    frame -> next = temporaryFrame -> next;
                    frameList -> head = frame;

                    free(temporaryFrame);
                // 處理選中是 tail 或中間時
                } else {
                    Frame *previousFrame = frameList -> head;
                    Frame *targetFrame;
                    Frame *nextFrame;

                    while(previousFrame -> next != longestDistanceFrame) {
                        previousFrame = previousFrame -> next;
                    }

                    // 處理選中是 tail 時
                    if(previousFrame -> next -> next == NULL) {
                        previousFrame -> next = frame;
                        frameList -> tail = frame;
                    // 處理選中是中間時
                    } else {
                        targetFrame = previousFrame -> next;
                        nextFrame = targetFrame -> next;

                        previousFrame -> next = frame;
                        frame -> next = nextFrame;

                        free(targetFrame);
                    }
                }

                // 處理 disk write
                if(frame -> dirty) {
                    frame -> dirty = 0;
                    diskWriteCount += 1;
                }
            }
        }
        pageFaultCount += 1;
    }

    result[0] = pageFaultCount;
    result[1] = interruptCount;
    result[2] = diskWriteCount;

    freeAllTheFrames(frameList);
}

void readFileAsReferenceString(int *referenceString){
    FILE *file;
    file = fopen("random.txt", "r");

    for(int i = 0; i < REFERENCE_TIME; i++) {
        fscanf(file, "%d, ", &referenceString[i]);
    }

    fclose(file);
}

void generateResultForConsole(int *referenceString, int *frameSize) {
    int results[3];

    for(int i = 0; i < 3; i++) {
        switch(i) {
            case 0:
                // 使用 Random
                printf("###################################################\n");
                printf("Using -> Random\n");
                processReferenceStringWithRandom(referenceString);
                checkReferenceString(referenceString);
                printf("###################################################\n\n");

                break;
            case 1:
                // 使用 Locality
                printf("###################################################\n");
                printf("Using -> Locality\n");
                processReferenceStringWithLocality(referenceString);
                checkReferenceString(referenceString);
                printf("###################################################\n\n");

                break;
            case 2:
                // 使用 Sequence And Jump
                printf("###################################################\n");
                printf("Using -> Sequence And Jump\n");
                processReferenceStringWithSequenceAndJump(referenceString);
                checkReferenceString(referenceString);
                printf("###################################################\n\n");

                break;
        }

        for(int j = 0; j < 5; j++) {
            printf("Frame Size: [%d]\n", frameSize[j]);
            printf("---------------------------------------------------\n");
            printf("Algorithm   Page Faults   Interrupts    Disk Writes\n");
            printf("---------------------------------------------------");
            pageReplacementWithFIFO(referenceString, frameSize[j], results);
            printf("\nFIFO     ");
            printf("        %6d", results[0]);
            printf("       %6d", results[1]);
            printf("         %6d", results[2]);
            pageReplacementWithEnhanced(referenceString, frameSize[j], results);
            printf("\nEnhanced  ");
            printf("        %6d", results[0]);
            printf("       %6d", results[1]);
            printf("         %6d", results[2]);
            pageReplacementWithARB(referenceString, frameSize[j], results);
            printf("\nARB      ");
            printf("        %6d", results[0]);
            printf("       %6d", results[1]);
            printf("         %6d", results[2]);
            pageReplacementWithDistance(referenceString, frameSize[j], results);
            printf("\nDistance ");
            printf("        %6d", results[0]);
            printf("       %6d", results[1]);
            printf("         %6d", results[2]);
            printf("\n\n");
        }
    }
}

void generateResultForGraph(int *referenceString, int *frameSize) {
    // 產生圖表用
    // Page Faults -> Interrupts -> Disk Writes
    // Random -> Locality -> Sequence And Jump
    // (Frame Size, FIFO, ARB, Enhanced second-chance, Distance)
    // 結果存至 ./output.txt

    // 全部結果
    int results[5][12][3];

    // FIFO
    int randomWithFIFO[3];
    int localityWithFIFO[3];
    int sequenceAndJumpWithFIFO[3];

    // Enhanced
    int randomWithEnhanced[3];
    int localityWithEnhanced[3];
    int sequenceAndJumpWithEnhanced[3];

    // ARB
    int randomWithARB[3];
    int localityWithARB[3];
    int sequenceAndJumpWithARB[3];

    // Distance
    int randomWithDistance[3];
    int localityWithDistance[3];
    int sequenceAndJumpWithDistance[3];

    processReferenceStringWithRandom(referenceString);
    for(int i = 0; i < 5; i++) {
        pageReplacementWithFIFO(referenceString, frameSize[i], randomWithFIFO);
        pageReplacementWithEnhanced(referenceString, frameSize[i], randomWithEnhanced);
        pageReplacementWithARB(referenceString, frameSize[i], randomWithARB);
        pageReplacementWithDistance(referenceString, frameSize[i], randomWithDistance);

        for(int j = 0; j < 3; j++) {
            results[i][0][j] = randomWithFIFO[j];
            results[i][1][j] = randomWithEnhanced[j];
            results[i][2][j] = randomWithARB[j];
            results[i][3][j] = randomWithDistance[j];
        }
    }

    processReferenceStringWithLocality(referenceString);
    for(int i = 0; i < 5; i++) {
        pageReplacementWithFIFO(referenceString, frameSize[i], localityWithFIFO);
        pageReplacementWithEnhanced(referenceString, frameSize[i], localityWithEnhanced);
        pageReplacementWithARB(referenceString, frameSize[i], localityWithARB);
        pageReplacementWithDistance(referenceString, frameSize[i], localityWithDistance);

        for(int j = 0; j < 3; j++) {
            results[i][4][j] = localityWithFIFO[j];
            results[i][5][j] = localityWithEnhanced[j];
            results[i][6][j] = localityWithARB[j];
            results[i][7][j] = localityWithDistance[j];
        }
    }

    processReferenceStringWithSequenceAndJump(referenceString);
    for(int i = 0; i < 5; i++) {
        pageReplacementWithFIFO(referenceString, frameSize[i], sequenceAndJumpWithFIFO);
        pageReplacementWithEnhanced(referenceString, frameSize[i], sequenceAndJumpWithEnhanced);
        pageReplacementWithARB(referenceString, frameSize[i], sequenceAndJumpWithARB);
        pageReplacementWithDistance(referenceString, frameSize[i], sequenceAndJumpWithDistance);

        for(int j = 0; j < 3; j++) {
            results[i][8][j] = sequenceAndJumpWithFIFO[j];
            results[i][9][j] = sequenceAndJumpWithEnhanced[j];
            results[i][10][j] = sequenceAndJumpWithARB[j];
            results[i][11][j] = sequenceAndJumpWithDistance[j];
        }
    }

    FILE *file;
    file = fopen("output.txt", "w");

    // Page Faults -> Interrupts -> Disk Writes
    for(int i = 0; i < 3; i++) {
        // Frame Sizes: 20 -> 40 -> 60 -> 80 -> 100
        for(int x = 0; x < 5; x++) {
            // Random -> Locality -> Sequence And Jump
            for(int y = 0; y < 12; y++){
                fprintf(file, "%d ", frameSize[x]);
                fprintf(file, "%d\n", results[x][y][i]);
            }
        }
    }

    fclose(file);
}

int main() {
    int referenceString[REFERENCE_TIME];
    int frameSize[5] = {20, 40, 60, 80, 100};

    srand(time(NULL));

    generateResultForConsole(referenceString, frameSize);
    generateResultForGraph(referenceString, frameSize);

    printf("\n");

    return 0;
}
