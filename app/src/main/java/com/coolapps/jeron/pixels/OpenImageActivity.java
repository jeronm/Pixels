package com.coolapps.jeron.pixels;

import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.Color;
import android.media.Image;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.provider.MediaStore;
import android.support.annotation.RequiresApi;
import android.support.design.widget.FloatingActionButton;
import android.support.design.widget.Snackbar;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.View;
import android.widget.ImageView;


import java.io.IOException;
import java.lang.reflect.Array;
import java.util.ArrayList;
import java.util.List;

public class OpenImageActivity extends AppCompatActivity {

    private int PICK_IMAGE_REQUEST = 1;
    Bitmap bitmap;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_open_image);
        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        FloatingActionButton fab = (FloatingActionButton) findViewById(R.id.fab);
        fab.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                imageNegative(view);
                Snackbar.make(view, "Image negative produced!", Snackbar.LENGTH_LONG)
                        .setAction("Action", null).show();
            }
        });

        FloatingActionButton fab2 = (FloatingActionButton) findViewById(R.id.fab2);
        fab2.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                abstractImage(view);
                Snackbar.make(view, "Abstract image produced!", Snackbar.LENGTH_LONG)
                        .setAction("Action", null).show();
            }
        });


        Intent intent = new Intent();
        // Show only images, no videos or anything else
        intent.setType("image/*");
        intent.setAction(Intent.ACTION_GET_CONTENT);
        // Always show the chooser (if there are multiple options available)
        startActivityForResult(Intent.createChooser(intent, "Select Picture"), PICK_IMAGE_REQUEST);
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);

        if (requestCode == PICK_IMAGE_REQUEST && resultCode == RESULT_OK && data != null && data.getData() != null) {

            Uri uri = data.getData();

            try {
                bitmap = MediaStore.Images.Media.getBitmap(getContentResolver(), uri);
                // Log.d(TAG, String.valueOf(bitmap));

                ImageView imageView = (ImageView) findViewById(R.id.imageView);
                imageView.setImageBitmap(bitmap);
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    /**
     * Creates an image negative from the user-selected image
     * @param view
     */
    public void imageNegative (View view) {
        Bitmap negativeBitmap = Bitmap.createBitmap(bitmap.getWidth(), bitmap.getHeight(), Bitmap.Config.ARGB_8888);
        for (int x = 0; x < bitmap.getWidth(); x++) {
            for (int y = 0; y < bitmap.getHeight(); y++) {
                int pixel = bitmap.getPixel(x, y);
                int red = 255 - Color.red(pixel);
                int green = 255 - Color.green(pixel);
                int blue = 255 - Color.blue(pixel);

                negativeBitmap.setPixel(x, y, Color.rgb(red, green, blue));
            }
        }
        ImageView imageView = (ImageView) findViewById(R.id.imageViewPostEdit);
        imageView.setImageBitmap(negativeBitmap);
    }


    /**
     * Creates an abstract image from the user-selected image. Like-colored pixels will be merged
     * and set to an average color of the corresponding group.
     * @param view
     */
    public void abstractImage (View view) {
        Bitmap abstractBitmap = Bitmap.createBitmap(bitmap.getWidth(), bitmap.getHeight(), Bitmap.Config.ARGB_8888);

        class Node {
            int pixel;
            int x;
            int y;

            public Node(int pixel, int x, int y) {
                this.pixel = pixel;
                this.x = x;
                this.y = y;
            }
        }

        for (int x = 0; x < bitmap.getWidth(); x++) {
            for (int y = 0; y < bitmap.getHeight(); y++) {
                int curPixel = abstractBitmap.getPixel(x, y);
                if (curPixel == 0) {
                    ArrayList<int[]> pixels = new ArrayList<int[]>();
                    findPixels(pixels, bitmap, abstractBitmap, bitmap.getPixel(x,y), x, y);
                    if (pixels.size() > 0) {
                        int averageColor = averageColor(pixels);
                        paintBitmap(pixels, abstractBitmap, averageColor);
                    }
                }
            }
        }

        ImageView imageView = (ImageView) findViewById(R.id.imageViewPostEdit);
        imageView.setImageBitmap(abstractBitmap);


    }

    public void findPixels (ArrayList<int[]> pixels, Bitmap iBitmap, Bitmap oBitmap,
                            int seedPixel, int x, int y) {
        if (x < 0 || x >= iBitmap.getWidth() || y < 0 || y >= iBitmap.getHeight()) return;

        int curIPixel = iBitmap.getPixel(x, y);
        int curOPixel = oBitmap.getPixel(x, y);

        if (curOPixel != 0) return;
        //double percentDiff = (double)(Math.abs(curIPixel) - Math.abs(seedPixel)) / Math.abs(seedPixel) * 100;
        //if (percentDiff >= 5) return;

        // seed pixel components
        int sRed = Color.red(seedPixel);
        int sBlue = Color.blue(seedPixel);
        int sGreen = Color.green(seedPixel);

        // current input pixel components
        int red = Color.red(curIPixel);
        int blue = Color.blue(curIPixel);
        int green = Color.green(curIPixel);

        if ((Math.abs(sRed - red) + Math.abs(sBlue - blue) + Math.abs(sGreen - green)) >= 100) return;

        oBitmap.setPixel(x, y, Color.rgb(255, 0, 0));

        int[] pixel = new int[3];
        pixel[0] = curIPixel;
        pixel[1] = x;
        pixel[2] = y;

        pixels.add(pixel);

        findPixels(pixels, iBitmap, oBitmap, seedPixel, x + 1, y);
        findPixels(pixels, iBitmap, oBitmap, seedPixel, x - 1, y);
        findPixels(pixels, iBitmap, oBitmap, seedPixel, x, y + 1);
        findPixels(pixels, iBitmap, oBitmap, seedPixel, x, y - 1);
    }

    public void paintBitmap (ArrayList<int[]> pixels, Bitmap oBitmap, int averageColor) {
        while(pixels.size() > 0) {
            int[] tempPixel = pixels.remove(0);
            oBitmap.setPixel(tempPixel[1], tempPixel[2], averageColor);
        }

    }

    public int averageColor (ArrayList<int[]> pixels) {
        int totalRed = 0;
        int totalBlue = 0;
        int totalGreen = 0;
        for (int i = 0; i < pixels.size(); i++) {
            totalRed += Color.red(pixels.get(i)[0]);
            totalBlue += Color.blue(pixels.get(i)[0]);
            totalGreen += Color.green(pixels.get(i)[0]);
        }
        return Color.rgb(totalRed / pixels.size(), totalGreen / pixels.size(),
                totalBlue / pixels.size());
    }

}
