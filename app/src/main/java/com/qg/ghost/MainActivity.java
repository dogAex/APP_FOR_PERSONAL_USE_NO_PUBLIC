package com.qg.ghost;

import android.app.Activity;
import android.os.Bundle;
import android.widget.TextView;
import android.content.pm.PackageManager;
import rikka.shizuku.Shizuku;

public class MainActivity extends Activity {
    static { 
        System.loadLibrary("qgghost"); 
    }
    private native String probeVulkan();
    private static final int SHIZUKU_REQUEST_CODE = 1003;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        TextView tv = new TextView(this);
        tv.setTextSize(18);
        tv.setPadding(50, 50, 50, 50);
        setContentView(tv);
        tv.setText("=== QG System Ghost 启动中 ===\n等待 Shizuku 响应...");

        try {
            if (Shizuku.pingBinder()) {
                if (Shizuku.checkSelfPermission() == PackageManager.PERMISSION_GRANTED) {
                    tv.setText(probeVulkan());
                } else {
                    tv.setText("⚠️ 需要 Shizuku 授权...");
                    Shizuku.requestPermission(SHIZUKU_REQUEST_CODE);
                }
            } else {
                tv.setText("❌ Shizuku 服务未启动！\n请先去后台打开 Shizuku App。");
            }
        } catch (Throwable t) {
            tv.setText("⚠️ Shizuku 接口异常: " + t.toString());
        }
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults) {
        if (requestCode == SHIZUKU_REQUEST_CODE && grantResults.length > 0 && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
            recreate();
        }
    }
}