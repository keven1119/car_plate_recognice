package com.carben.carben;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;
import androidx.constraintlayout.widget.ConstraintLayout;
import androidx.core.app.ActivityCompat;

import android.Manifest;
import android.content.Intent;
import android.database.Cursor;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Color;
import android.net.Uri;
import android.os.Bundle;
import android.provider.MediaStore;
import android.view.View;
import android.view.ViewGroup;
import android.widget.FrameLayout;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import com.carben.carplate.OnPlateMsgListener;
import com.carben.carplate.RecognicePlateHelper;

public class MainActivity extends AppCompatActivity implements OnPlateMsgListener {

    // Used to load the 'native-lib' library on application startup.


    final static int SELECT_ORIGINAL_PIC = 111;
    ConstraintLayout picContainer;
    ImageView picView;
    TextView addCover;
    TextView removeCover;

    FrameLayout coverContainer;

    Bitmap currentBitmap;
    String picFilePath;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        // Example of a call to a native method
        picContainer = findViewById(R.id.pic_container);
        picView = findViewById(R.id.imgShow);
        addCover = findViewById(R.id.add_cover);
        removeCover = findViewById(R.id.remove_cover);
        coverContainer = findViewById(R.id.cover_container);

        picContainer.postDelayed(new Runnable() {
            @Override
            public void run() {

                ActivityCompat.requestPermissions(MainActivity.this,
                        new String[]{Manifest.permission.WRITE_EXTERNAL_STORAGE, Manifest.permission.READ_EXTERNAL_STORAGE}, 111);

            }
        },2000);

        picContainer.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                selectFromGallery();
            }
        });
        addCover.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                String realPath = picFilePath.replace("/storage/emulated/0", "sdcard");

                new RecognicePlateHelper().getPlateMsg(
                        "sdcard/HOG_SVM_DATA2.xml",
                        "sdcard/HOG_ANN_ZH_DATA2.xml",
                        "sdcard/HOG_ANN_DATA2.xml",
                        realPath,
                        MainActivity.this);
            }
        });

        removeCover.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                coverContainer.removeAllViews();
            }
        });
    }



    /**
     * 从相册选择原生的照片（不裁切）
     */
    private void selectFromGallery() {
        Intent intent=new Intent();
        intent.setAction(Intent.ACTION_PICK);//Pick an item fromthe data
        intent.setType("image/*");//从所有图片中进行选择
        startActivityForResult(intent, SELECT_ORIGINAL_PIC);
    }


    @Override
    protected void onActivityResult(int requestCode, int resultCode, @Nullable Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        switch (requestCode) {
            case SELECT_ORIGINAL_PIC:
                if (resultCode==RESULT_OK) {//从相册选择照片不裁切
                    try {
                        Uri selectedImage = data.getData(); //获取系统返回的照片的Uri
                        String[] filePathColumn = { MediaStore.Images.Media.DATA };
                        Cursor cursor =getContentResolver().query(selectedImage,
                                filePathColumn, null, null, null);//从系统表中查询指定Uri对应的照片
                        cursor.moveToFirst();
                        int columnIndex = cursor.getColumnIndex(filePathColumn[0]);
                        picFilePath = cursor.getString(columnIndex);  //获取照片路径
                        cursor.close();

                        if(currentBitmap != null){
                            currentBitmap.recycle();
                        }

                        currentBitmap= BitmapFactory.decodeFile(picFilePath);
                        setImageSize(currentBitmap);
                        picView.setImageBitmap(currentBitmap);
                        coverContainer.removeAllViews();


                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                }
                break;
        }
    }

    private void setImageSize(Bitmap bitmap){
        int width = bitmap.getWidth();
        int height = bitmap.getHeight();

        int maxEdge = picContainer.getWidth();

        ViewGroup.LayoutParams layoutParams = picView.getLayoutParams();
        if(width > height){
            layoutParams.width = maxEdge;
            layoutParams.height = maxEdge * height/width;
        }else if (height > width){
            layoutParams.height = maxEdge;
            layoutParams.width = maxEdge * width/height;
        }else {
            layoutParams.width = maxEdge;
            layoutParams.height = maxEdge;
        }

        picView.setLayoutParams(layoutParams);
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();


    @Override
    public void onParsePlateFail() {

    }

    @Override
    public void onParsePlateSuc(final String plateNum, final float offsetCenterX, final float offsetCenterY, final float offsetWidth, final float offsetHeight, final float angle) {
        ImageView imageView = new ImageView(this);
        imageView.setBackgroundColor(Color.RED);


        int containerWidth = coverContainer.getWidth();
        int orignalWidth = currentBitmap.getWidth();

        float ratio =  (float) containerWidth/ orignalWidth;

        int coverWidth = (int)(offsetWidth * ratio);
        int coverHeight = (int)(offsetHeight * ratio);


        int coverMarginLeft = (int)( offsetCenterX * ratio - (float)coverWidth/2);
        int coverMarginTop = (int)(offsetCenterY * ratio - (float)coverHeight/2);

        coverContainer.addView(imageView);

        ViewGroup.MarginLayoutParams layoutParams = (ViewGroup.MarginLayoutParams)imageView.getLayoutParams();

        layoutParams.leftMargin = coverMarginLeft;
        layoutParams.topMargin = coverMarginTop;

        layoutParams.width = coverWidth;
        layoutParams.height = coverHeight;

        imageView.setLayoutParams(layoutParams);
        imageView.setPivotX(coverWidth/2f);
        imageView.setPivotY(coverHeight/2f);
        imageView.setRotation(angle);


        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Toast.makeText(MainActivity.this,
                        plateNum +"\n" +
                        angle +"\n" +
                        offsetCenterX +"\n" +
                        offsetCenterY +"\n" +
                        offsetWidth +"\n" +
                        offsetHeight +"\n" ,Toast.LENGTH_LONG)
                        .show();
            }
        });
    }
}
