package com.example.xieyao.test;

import java.util.Iterator;
import java.util.List;
import java.io.File;
import java.io.RandomAccessFile;
import android.app.Activity;
import android.content.Context;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;
import java.io.ByteArrayOutputStream;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;
import android.os.Environment;
import java.text.DecimalFormat;

public class Test extends Activity {
    /** Called when the activity is first created. */
    //设置LOG标签
    private static final String TAG = "sensor";
    private  SensorManager sm;

    TextView View1 = null;
    TextView View2 = null;
    TextView View3 = null;

    boolean start = false;
    boolean record = true;
    private String fileName = "sensor_data.txt";
    float[][][] data = new float[10][10][100];
    int[][] code = new int[10][20];
    int group_count = 0;
    int data_count = 0;

    FileOutputStream outputStream = null;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_test);

        View1 = (TextView) findViewById(R.id.edt1);
        View2 = (TextView) findViewById(R.id.edt2);
        View3 = (TextView) findViewById(R.id.edt3);

        Button addButton = (Button) this.findViewById(R.id.addButton);
        addButton.setOnClickListener(listener);
        Button encodeButton = (Button) this.findViewById(R.id.encodeButton);
        encodeButton.setOnClickListener(listener);

        for(int i = 0;i<10;i++){
            for(int j = 0;j<10;j++){
                for(int k = 0;k<100;k++){
                    data[i][j][k] = 0;
                }
            }
            for(int j = 0;j<20;j++){
                code[i][j] = 0;
            }
        }

        //创建一个SensorManager来获取系统的传感器服务
        sm = (SensorManager)getSystemService(Context.SENSOR_SERVICE);
        /*
         * 最常用的一个方法 注册事件
         * 参数1 ：SensorEventListener监听器
         * 参数2 ：Sensor 一个服务可能有多个Sensor实现，此处调用getDefaultSensor获取默认的Sensor
         * 参数3 ：模式 可选数据变化的刷新频率
         * */
        int sensorType = Sensor.TYPE_ACCELEROMETER;
        sm.registerListener(myListener,sm.getDefaultSensor(sensorType),SensorManager.SENSOR_DELAY_UI);
        int sensorType2 = Sensor.TYPE_LINEAR_ACCELERATION;
        sm.registerListener(myListener,sm.getDefaultSensor(sensorType2),SensorManager.SENSOR_DELAY_UI);
        int sensorType3 = Sensor.TYPE_GYROSCOPE;
        sm.registerListener(myListener,sm.getDefaultSensor(sensorType3),SensorManager.SENSOR_DELAY_UI);
        int sensorType4 = Sensor.TYPE_PROXIMITY;
        sm.registerListener(myListener,sm.getDefaultSensor(sensorType4),SensorManager.SENSOR_DELAY_UI);

        try {
            outputStream = openFileOutput(fileName,Activity.MODE_PRIVATE);
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }

    }

    // 声明监听器
    private View.OnClickListener listener = new OnClickListener() {
        public void onClick(View v) {
            Button view = (Button) v;
            switch (view.getId()) {
                case R.id.addButton:
                    if(start == false){
                        start = true;
                    }
                    else{
                        //start = false;
                        //group_count++;
                    }
                    break;
                case R.id.encodeButton:
                    encode();
                    break;
            }
        }
    };

    private void encode(){
        for(int i = 0;i<10;i++){
            int codepos = 0;
            for(int j = 0;j<10;j++){
                encode_group(i,j,1,1,codepos);
            }
        }
    }

    private void encode_group(int group, int which, int maxcount, int mincount, int codepos){
        int start = 0;
        int end = 99;
        while(data[group][which][start]==0) start++;
        while(data[group][which][end]==0) end--;
        int[] max_time = new int[maxcount];
        float[] max_value = new float[maxcount];
        int[] min_time = new int[mincount];
        float[] min_value = new float[mincount];
        for(int i = 0;i<maxcount;i++){
            max_time[i] = 0;
            max_value[i] = -20;
        }
        for(int i = 0;i<mincount;i++){
            min_time[i] = 0;
            min_value[i] = 20;
        }
        boolean record_max = false;
        boolean record_min = false;
        for(int i = start+1;i<=end;i++){
            if(data[group][which][i]>data[group][which][i-1]) record_max = true;
            if(data[group][which][i]<data[group][which][i-1]) record_min = true;
            if (data[group][which][i] < data[group][which][i - 1] && record_max){
                record_max = false;
                int pos = 0;
                for (int j = 0; j < maxcount; j++){
                    if (max_value[j] < max_value[pos]) pos = j;
                }
                if (data[group][which][i - 1] > max_value[pos]){
                    max_value[pos] = data[group][which][i - 1];
                    max_time[pos] = i - 1;
                }
            }
            if (data[group][which][i] > data[group][which][i - 1] && record_min){
                record_min = false;
                int pos = 0;
                for (int j = 0; j < mincount; j++){
                    if (min_value[j]>min_value[pos]) pos = j;
                }
                if (data[group][which][i - 1] < min_value[pos]){
                    min_value[pos] = data[group][which][i - 1];
                    min_time[pos] = i - 1;
                }
            }
        }
        for(int i = 0;i<maxcount;i++){
            code[group][codepos] = max_time[i];
            codepos++;
        }
        for(int i = 0;i<mincount;i++){
            code[group][codepos] = min_time[i];
            codepos++;
        }
    }

    // write files to SD card
    private void info(String infos){
        String content = infos + "\n";
        try {
            outputStream.write(content.getBytes());
            //获取SD卡的目录
            File sdCardDir = Environment.getExternalStorageDirectory();
            File targetFile = new File(sdCardDir.getPath() + '/' + fileName);
            RandomAccessFile raf = new RandomAccessFile(targetFile , "rw");
            raf.seek(targetFile.length());
            raf.write(content.getBytes());
            raf.close();
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private void save(float Ax,float Ay,float Az,float LAx,float LAy,float LAz,float Gx,float Gy,float Gz,float P) {

        DecimalFormat df = new DecimalFormat("0.0000");
        String content = df.format(Ax)+"\t" +df.format(Ay)+"\t" +df.format(Az)
                +"\t"+df.format(LAx)+"\t"+df.format(LAy)+"\t"+df.format(LAz)
                +"\t"+df.format(Gx)+"\t"+df.format(Gy)+"\t"+df.format(Gz)
                +"\t"+df.format(P)+"\n";

        try {
            outputStream.write(content.getBytes());
            //获取SD卡的目录
            File sdCardDir = Environment.getExternalStorageDirectory();
            File targetFile = new File(sdCardDir.getPath() + '/' + fileName);
            RandomAccessFile raf = new RandomAccessFile(targetFile , "rw");
            raf.seek(targetFile.length());
            raf.write(content.getBytes());
            raf.close();
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    /*
     * SensorEventListener接口的实现，需要实现两个方法
     * 方法1 onSensorChanged 当数据变化的时候被触发调用
     * 方法2 onAccuracyChanged 当获得数据的精度发生变化的时候被调用，比如突然无法获得数据时
     * */
    final SensorEventListener myListener = new SensorEventListener(){

        float Accelerometer_x;
        float Accelerometer_y;
        float Accelerometer_z;
        float Linear_Acceleration_x;
        float Linear_Acceleration_y;
        float Linear_Acceleration_z;
        float Gyroscope_x;
        float Gyroscope_y;
        float Gyroscope_z;
        float Proximity;

        //复写onSensorChanged方法
        public void onSensorChanged(SensorEvent sensorEvent){
            /*
            if(sensorEvent.sensor.getType() == Sensor.TYPE_ACCELEROMETER && !record){
                Accelerometer_z = sensorEvent.values[2];
                if(Math.abs(Accelerometer_z)>5){
                    record = true;
                    info("start recording");
                }
            }
            */
            if(sensorEvent.sensor.getType() == Sensor.TYPE_ACCELEROMETER && start && record){
                Accelerometer_x = sensorEvent.values[0];
                Accelerometer_y = sensorEvent.values[1];
                Accelerometer_z = sensorEvent.values[2];
                /*
                if(Math.abs(Accelerometer_z)<5){
                    record = false;
                    info("end recording");
                }
                */
                View1.setText("X:"+Accelerometer_x);
                View2.setText("Y:"+Accelerometer_y);
                View3.setText("Z:"+Accelerometer_z);
            }
            if(sensorEvent.sensor.getType() == Sensor.TYPE_LINEAR_ACCELERATION && start && record){
                Linear_Acceleration_x = sensorEvent.values[0];
                Linear_Acceleration_y = sensorEvent.values[1];
                Linear_Acceleration_z = sensorEvent.values[2];
            }
            if(sensorEvent.sensor.getType() == Sensor.TYPE_GYROSCOPE && start && record){
                Gyroscope_x = sensorEvent.values[0];
                Gyroscope_y = sensorEvent.values[1];
                Gyroscope_z = sensorEvent.values[2];
            }
            if(sensorEvent.sensor.getType() == Sensor.TYPE_PROXIMITY && start && record){
                Proximity = sensorEvent.values[0];
            }
            if(start && record) {
                save(Accelerometer_x, Accelerometer_y, Accelerometer_z, Linear_Acceleration_x, Linear_Acceleration_y, Linear_Acceleration_z,
                        Gyroscope_x, Gyroscope_y, Gyroscope_z, Proximity);
                data[group_count][0][data_count] = Accelerometer_x;
                data[group_count][1][data_count] = Accelerometer_y;
                data[group_count][2][data_count] = Accelerometer_z;
                data[group_count][3][data_count] = Linear_Acceleration_x;
                data[group_count][4][data_count] = Linear_Acceleration_y;
                data[group_count][5][data_count] = Linear_Acceleration_z;
                data[group_count][6][data_count] = Gyroscope_x;
                data[group_count][7][data_count] = Gyroscope_y;
                data[group_count][8][data_count] = Gyroscope_z;
                data[group_count][9][data_count] = Proximity;
                data_count++;
                if(data_count == 100){
                    if(group_count==10) group_count = 0;
                    else group_count++;
                    start = false;
                }
            }
        }
        //复写onAccuracyChanged方法
        public void onAccuracyChanged(Sensor sensor , int accuracy){
            //Log.i(TAG, "onAccuracyChanged");
        }
    };

    public void onPause(){
        /*
         * 很关键的部分：注意，说明文档中提到，即使activity不可见的时候，感应器依然会继续的工作，测试的时候可以发现，没有正常的刷新频率
         * 也会非常高，所以一定要在onPause方法中关闭触发器，否则讲耗费用户大量电量，很不负责。
         * */
        sm.unregisterListener(myListener);
        super.onPause();
    }

}