package com.dragon4.owo.somaiotapp.Model;

/**
 * Created by joyeongje on 2016. 11. 18..
 */

public class Monitor {


    private String macAddress;
    private String ipAddress;
    private String hostName;
    private String rsByte;
    private String csByte;
    private String connectTime;
    private String connectOutTime;

    public Monitor() {

    }

    public Monitor(String macAddress,String ipAddress, String hostName,String rsByte,String csByte,String connectTime,String connectOutTime) {

        this.macAddress = macAddress;
        this.ipAddress = ipAddress;
        this.hostName = hostName;
        this.rsByte = rsByte;
        this.csByte = csByte;
        this.connectTime = connectTime;
        this.connectOutTime = connectOutTime;
    }

    public String getMacAddress() {
        return macAddress;
    }

    public void setMacAddress(String macAddress) {
        this.macAddress = macAddress;
    }

    public String getIpAddress() {
        return ipAddress;
    }

    public void setIpAddress(String ipAddress) {
        this.ipAddress = ipAddress;
    }

    public String getHostName() {
        return hostName;
    }

    public void setHostName(String hostName) {
        this.hostName = hostName;
    }

    public String getRsByte() {
        return rsByte;
    }

    public void setRsByte(String rsByte) {
        this.rsByte = rsByte;
    }

    public String getCsByte() {
        return csByte;
    }

    public void setCsByte(String csByte) {
        this.csByte = csByte;
    }

    public String getConnectTime() {
        return connectTime;
    }

    public void setConnectTime(String connectTime) {
        this.connectTime = connectTime;
    }

    public String getConnectOutTime() {
        return connectOutTime;
    }

    public void setConnectOutTime(String connectOutTime) {
        this.connectOutTime = connectOutTime;
    }
}
