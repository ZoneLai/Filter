package com.zone.triangle;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.Toast;

public class MainActivity extends AppCompatActivity {
    TriangleSurfaceView mTriangleSurfaceView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
//        setContentView(R.layout.activity_main);
        mTriangleSurfaceView = new TriangleSurfaceView(this, null);
        setContentView(mTriangleSurfaceView);
    }

    @Override
    protected void onStop() {
        super.onStop();
        mTriangleSurfaceView.onStop();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        mTriangleSurfaceView.onDestroy();
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.menu_filter, menu);
        return super.onCreateOptionsMenu(menu);
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId()) {
            case R.id.mDefault:
                mTriangleSurfaceView.setFilterMode(0);
                break;
            case R.id.mGray:
                mTriangleSurfaceView.setFilterMode(1);
                break;
            case R.id.mCool:
                mTriangleSurfaceView.setFilterMode(2);
                break;
            case R.id.mWarm:
                mTriangleSurfaceView.setFilterMode(3);
                break;
            case R.id.mBlur:
                mTriangleSurfaceView.setFilterMode(4);
                break;
            case R.id.mMagn:
                mTriangleSurfaceView.setFilterMode(5);
                break;
        }
        return super.onOptionsItemSelected(item);
    }
}
