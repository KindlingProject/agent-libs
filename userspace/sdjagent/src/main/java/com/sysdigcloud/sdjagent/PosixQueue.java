package com.sysdigcloud.sdjagent;

import java.io.IOException;
import java.util.logging.Logger;

/**
 * Created by luca on 18/05/16.
 */
public class PosixQueue {
    private static final Logger LOGGER = Logger.getLogger(PosixQueue.class.getName());

    public enum Direction {
        SEND(0), RECEIVE(1);

        private final int value;
        Direction(int value) {
            this.value = value;
        }
    }

    static {
        if (CLibrary.libraryLoaded) {
            if(!setQueueLimits()) {
                LOGGER.warning("Cannot set queue limits");
            }
        }
    }

    private final int fd;
    private final String name;
    public PosixQueue(String name, Direction direction) throws IOException {
        this.name = name;
        if (CLibrary.libraryLoaded) {
            int res = openQueue(name, direction.value, 1);
            if (res > 0) {
                this.fd = res;
            } else {
                throw new IOException(String.format("Cannot create posix queue %s errno=%d", name, -res));
            }
        } else {
            throw new IOException(String.format("Cannot create posix queue %s (libsdjagentjni not loaded)", name));
        }
    }

    public boolean send(String message) {
        int res = queueSend(fd, message);
        switch(res) {
            case 0:
                break;
            case -1:
                LOGGER.fine(String.format("Cannot send on queue %s, is full", this.name));
                break;
            case -2:
                LOGGER.warning(String.format("Cannot send on queue %s, msg too big", this.name));
                break;
            default:
                LOGGER.warning(String.format("Cannot send on queue %s, errno: %d", this.name, res));
                break;
        }
        return res == 0;
    }

    public String receive(long timeout_s) {
        byte[] data = queueReceive(fd, timeout_s);
        if(data != null) {
            return new String(data);
        } else {
            return null;
        }
    }

    public void close() {
        queueClose(fd);
    }

    private static native boolean setQueueLimits();
    private static native int openQueue(String name, int flags, int maxmsgs);
    private static native int queueSend(int fd, String message);
    private static native byte[] queueReceive(int fd, long timeout_s);
    private static native boolean queueClose(int fd);
}
