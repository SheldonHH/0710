import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class Satellite {
    private List<GroundStation> groundStations;
    private Lock lock;
    private String language;

    public Satellite(String language) {
        this.groundStations = new ArrayList<>();
        this.lock = new ReentrantLock();
        this.language = language;
        if (language.equals("cn")) {
            System.out.println("创建 Satellite 对象，使用列表来存储地面站。");
        } else {
            System.out.println("Satellite created, using list to store ground stations.");
        }
    }

    public void addGroundStation(GroundStation groundStation) {
        lock.lock();
        try {
            groundStations.add(groundStation);
            if (language.equals("cn")) {
                System.out.println("UUID 为: " + groundStation.getUUID() + " 的地面站已添加。");
            } else {
                System.out.println("Ground station with UUID: " + groundStation.getUUID() + " added.");
            }
        } finally {
            lock.unlock();
        }
    }

    public void notifyGroundStations() {
        lock.lock();
        try {
            if (language.equals("cn")) {
                System.out.println("通知所有地面站...");
            } else {
                System.out.println("Notifying ground stations...");
            }
            for (GroundStation gs : groundStations) {
                if (gs != null) {
                    if (language.equals("cn")) {
                        System.out.println("通知 UUID 为: " + gs.getUUID() + " 的地面站。");
                    } else {
                        System.out.println("Notifying ground station with UUID: " + gs.getUUID() + ".");
                    }
                    gs.receiveData();
                }
            }
        } finally {
            lock.unlock();
        }
    }
}
