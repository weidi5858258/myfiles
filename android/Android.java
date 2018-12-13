public class Android{


}

// ReadWriteLock锁
class Data {
	private int data;
	/***
	读与写、写与写互斥，但是读与读不互斥，
	这就是ReadWriteLock带来的好处，
	它在保证共享数据并发操作的完整性和一致性，
	最重要的是提高了读写的效率
	*/
	private ReadWriteLock readWriteLock = new ReentrantReadWriteLock();

	public void set(int data) throws Exception {
		// 获取写锁
		readWriteLock.writeLock().lock(); 
		try {
			System.out.println(Thread.currentThread().getName() + "准备写入数据...");
			Thread.sleep(50); // 模拟耗时操作
			this.data = data;
			System.out.println(Thread.currentThread().getName() + "写入数据成功!");
		} finally {
			// 释放写锁
			readWriteLock.writeLock().unlock(); 
		}

	}

	public void get() throws Exception {
		// 获取读锁
		readWriteLock.readLock().lock(); 
		try {
			System.out.println(Thread.currentThread().getName() + "准备读取数据...");
			Thread.sleep(50); // 模拟耗时操作
			System.out.println(Thread.currentThread().getName() + "读取数据:" + this.data);
		} finally {
			// 释放读锁
			readWriteLock.readLock().unlock(); 
		}

	}
}

// ReentrantLock锁
class TestLock {
	private ReentrantLock lock = null;

    public int data = 100;     // 用于线程同步访问的共享数据

    public TestLock() {
        lock = new ReentrantLock(); // 创建一个自由竞争的可重入锁
    }

    public ReentrantLock getLock() {
    	return lock;
    }
    
    public void testReentry() {
    	lock.lock();
    	Calendar now = Calendar.getInstance();
    	System.out.println(now.getTime() + " " + Thread.currentThread()    + " get lock.");
    }

    public void testRun() throws Exception {
    	lock.lock();

    	Calendar now = Calendar.getInstance();
    	try {
            // 获取锁后显示 当前时间 当前调用线程 共享数据的值（并使共享数据 + 1）
    		System.out.println(now.getTime() + " " + Thread.currentThread()+ " accesses the data " + data++);
    		Thread.sleep(1000);
    	} catch (Exception e) {
    		e.printStackTrace();
    	} finally {
    		lock.unlock();
    	}
    }

	/***
	在某些线程中使用这个类的对象调用read()方法时,
	如果不想再等待锁的到来,那么只要在其他地方使用
	这个类的对象调用interrupt()方法,就会退出这个
	read()方法.但是在调用read()方法时需要try-catch
	这个InterruptedException异常.如
	try {
        buff.read();//可以收到中断的异常，从而有效退出    
    } catch (InterruptedException e) {
        System.out.println("我不读了");
    }
	*/
    public void read() throws InterruptedException {
        lock.lockInterruptibly();// 注意这里，可以响应中断    
        try {
        	System.out.println("从这个buff读数据");
        } finally {
        	lock.unlock();
        }
    }

    public static void main(String[] args) {
    	TestLock tester = new TestLock();

        //1、测试可重入
    	tester.testReentry();
        tester.testReentry(); // 能执行到这里而不阻塞，表示锁可重入
        tester.testReentry(); // 再次重入

        // 释放重入测试的锁，要按重入的数量解锁，否则其他线程无法获取该锁。
        tester.getLock().unlock();
        tester.getLock().unlock();
        tester.getLock().unlock();

        //2、测试互斥
        // 启动3个线程测试在锁保护下的共享数据data的访问
        new Thread(new workerThread(tester)).start();
        new Thread(new workerThread(tester)).start();
        new Thread(new workerThread(tester)).start();
    }

	// 工作线程，调用TestServer.testRun
    private class workerThread implements Runnable {
    	private TestLock tester = null;

    	public workerThread(TestLock testLock) {
    		this.tester = testLock;
    	}

    	public void run() {
    		try {
    			tester.testRun();
    		} catch (Exception e) {
    			e.printStackTrace();
    		}
    	}
    }
}


/***
Android系统源代码中有用的知识点：
主要的代码在于D:\AOSP\frameworks\base\core\java\android
这个目录下面。

面向对象Fragment通信框架设计
1.

<activity
    android:name=".example.WindowPlayActivity"
    // activity生命周期不重建
    android:configChanges="orientation|keyboardHidden|screenSize|screenLayout|smallestScreenSize"
    android:label="可使用小窗口模式播放"
    // PIP模式
    android:resizeableActivity="true"
    android:supportsPictureInPicture="true"
    android:theme="@android:style/Theme.Translucent.NoTitleBar.Fullscreen"
    // 横屏
    android:screenOrientation="landscape"><!--portrait-->
</activity>


*/