import java.util.UUID;

public class GroundStation {
    private Satellite satellite;
    private String uuid;
    private String language;

    public GroundStation(Satellite satellite, String language) {
        this.satellite = satellite;
        this.language = language;
        this.uuid = UUID.randomUUID().toString();
        if (language.equals("cn")) {
            System.out.println("创建 GroundStation 对象，UUID 为: " + this.uuid);
        } else {
            System.out.println("GroundStation created with UUID: " + this.uuid);
        }
    }

    public void sendData() {
        if (language.equals("cn")) {
            System.out.println("地面站 " + this.uuid + " 正在向卫星发送数据...");
        } else {
            System.out.println("GroundStation " + this.uuid + " sending data to satellite...");
        }
        satellite.notifyGroundStations();
    }

    public void receiveData() {
        if (language.equals("cn")) {
            System.out.println("地面站 " + this.uuid + " 接收到来自卫星的数据。");
        } else {
            System.out.println("GroundStation " + this.uuid + " received data from satellite.");
        }
    }

    public String getUUID() {
        return this.uuid;
    }
}
