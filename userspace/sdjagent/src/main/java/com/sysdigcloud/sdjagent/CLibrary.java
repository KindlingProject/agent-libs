package com.sysdigcloud.sdjagent;

import java.io.FileInputStream;
import java.io.IOException;
import java.util.Scanner;
import java.util.logging.Logger;

/**
 * Created by luca on 08/01/15.
 */
public class CLibrary {
    private final static Logger LOGGER = Logger.getLogger(CLibrary.class.getName());
    private static boolean libraryLoaded;
    private static int pid;
    
    static {
        try {
            FileInputStream procStatusFile = new FileInputStream("/proc/self/status");
            Scanner procStatusReader = new Scanner(procStatusFile);
            while (procStatusReader.hasNextLine()) {
                String line = procStatusReader.nextLine();
                if (line.startsWith("Pid:")) {
                    // Example:
                    // Pid:	1020
                    // Uid: <pid>
                    String[] parsed = line.split("\\s+");
                    pid = Integer.parseInt(parsed[1]);
                    break;
                }
            }
        } catch (IOException ex)
        {
            LOGGER.severe(String.format("Error while reading /proc/self/status: %s", ex.getMessage()));
        }

        try {
            System.loadLibrary("sdjagentjni");
            libraryLoaded = true;
        } catch ( UnsatisfiedLinkError ex) {
            LOGGER.warning(String.format("Cannot load JNI library: %s", ex.getMessage()));
        }
    }

    public static int getPid() {
        return pid;
    }

    public static long[] getUidAndGid(int pid) throws IOException {
        FileInputStream procStatusFile = new FileInputStream(String.format("/proc/%d/status", pid));
        Scanner procStatusReader = new Scanner(procStatusFile);
        long[] result = new long[2];
        while (procStatusReader.hasNextLine())
        {
            String line = procStatusReader.nextLine();
            if (line.startsWith("Uid:")) {
                // Example:
                // Uid:	102	102	102	102
                // Uid: <real> <effective> <saved> <filesystem>
                String[] uids = line.split("\\s+");
                result[0] = Long.parseLong(uids[2]);
                String groupLine = procStatusReader.nextLine();
                String[] gids = groupLine.split("\\s+");
                result[1] = Long.parseLong(gids[2]);
                break;
            }
        }
        return result;
    }

    // Use long instead of int because
    // C interface uses uint32_t
    public static int seteuid(long euid) {
        if (libraryLoaded) {
            return real_seteuid(euid);
        } else {
            return -99;
        }
    }
    public static int setegid(long egid) {
        if (libraryLoaded) {
            return real_setegid(egid);
        } else {
            return -99;
        }
    }

    private static native int real_seteuid(long euid);
    private static native int real_setegid(long egid);

    private CLibrary() {
        // Deny create instances of this class
    }
}
