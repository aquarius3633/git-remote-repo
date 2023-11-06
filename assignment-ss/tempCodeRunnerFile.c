    // bufferSpaceEmpty = sem_open("/empty_sem", O_CREAT, 0644, BUFFER_SIZE);
    //     if (bufferSpaceEmpty == SEM_FAILED) {
    //         perror("Failed to open semphore for empty");
    //         exit(-1);
    //     }

    // bufferSpaceFull = sem_open("/full_sem", O_CREAT, 0644, 0);
    // if (bufferSpaceFull == SEM_FAILED) {
    //     sem_close(bufferSpaceEmpty);
    //     perror("Failed to open semphore for full");
    //     exit(-1);
    // }