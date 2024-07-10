public class Main {
    public static void main(String[] args) {
        String language = "en"; // 默认语言为英文
        if (args.length > 0) {
            language = args[0];
        }

        // 创建卫星对象
        Satellite satellite = new Satellite(language);

        // 创建多个地面站对象并添加到卫星
        GroundStation gs1 = new GroundStation(satellite, language);
        GroundStation gs2 = new GroundStation(satellite, language);
        satellite.addGroundStation(gs1);
        satellite.addGroundStation(gs2);

        // 创建并启动地面站线程
        Thread t1 = new Thread(() -> {
            try {
                Thread.sleep(1000); // 模拟一些延迟
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            gs1.sendData();
        });

        Thread t2 = new Thread(() -> {
            try {
                Thread.sleep(1000); // 模拟一些延迟
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            gs2.sendData();
        });

        t1.start();
        t2.start();

        // 等待线程完成
        try {
            t1.join();
            t2.join();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
}
