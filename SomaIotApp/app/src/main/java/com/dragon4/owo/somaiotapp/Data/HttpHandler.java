package com.dragon4.owo.somaiotapp.Data;

/**
 * Created by joyeongje on 2016. 11. 18..
 */

import android.os.AsyncTask;
import android.util.Log;

import java.io.BufferedInputStream;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;
import java.security.KeyManagementException;
import java.security.NoSuchAlgorithmException;
import java.security.SecureRandom;
import java.security.cert.CertificateException;
import java.security.cert.X509Certificate;

import javax.net.ssl.HostnameVerifier;
import javax.net.ssl.HttpsURLConnection;
import javax.net.ssl.SSLContext;
import javax.net.ssl.SSLSession;
import javax.net.ssl.X509TrustManager;


public class HttpHandler extends AsyncTask<String, Void, String> {
    private static final String TAG = "HttpHandler";

    @Override
    protected String doInBackground(String... params) {
        URL url = null;
        String responseStr = null;
        String urlStr = params[0];
        HttpURLConnection conn = null;

        if (urlStr.startsWith("file:\\")) ;
        // TODO: 2016. 10. 2. 파일에서 읽어올경우 처리해야됨

        if (urlStr.startsWith("https://")) {
            HttpsURLConnection.setDefaultHostnameVerifier(new HostnameVerifier() {
                public boolean verify(String hostname, SSLSession session) {
                    return true;
                }
            });

            // SSL 통신용 컨텍스트
            SSLContext context = null;
            try {
                context = SSLContext.getInstance("TLS");
            } catch (NoSuchAlgorithmException e) {
                e.printStackTrace();
            }

            try {
                context.init(null, new X509TrustManager[]{new X509TrustManager() {
                    public void checkClientTrusted(X509Certificate[] chain,
                                                   String authType) throws CertificateException {
                    }

                    public void checkServerTrusted(X509Certificate[] chain,
                                                   String authType) throws CertificateException {
                    }

                    public X509Certificate[] getAcceptedIssuers() {
                        return new X509Certificate[0];
                    }
                }}, new SecureRandom());


            } catch (KeyManagementException e) {
                e.printStackTrace();
            }

            HttpsURLConnection.setDefaultSSLSocketFactory(context.getSocketFactory());
        }


        try {

            url = new URL(urlStr);

            conn = (HttpURLConnection) url.openConnection();
            conn.setReadTimeout(10000);
            conn.setConnectTimeout(10000);
            conn.setRequestMethod("GET");

            //// TODO: 2016. 10. 12. json

            int responseCode = conn.getResponseCode();
            Log.i("ResponseCode : ", Integer.toString(responseCode));


            InputStream in = new BufferedInputStream(conn.getInputStream());
            responseStr = convertStreamToString(in);


        } catch (MalformedURLException e) {
            Log.e(TAG, "MalformedURLException: " + e.getMessage());
        } catch (IOException e) {
            Log.e(TAG, "IOException: " + e.getMessage());
        } catch (Exception e) {
            Log.e(TAG, "Exception: " + e.getMessage());
        }

        return responseStr;
    }


    private String convertStreamToString(InputStream is) {
        BufferedReader reader = new BufferedReader(new InputStreamReader(is));
        StringBuilder sb = new StringBuilder();
        String line;
        try {

            while ((line = reader.readLine()) != null) {
                sb.append(line + '\n');
            }
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            try {
                is.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }

        return sb.toString();
    }

}