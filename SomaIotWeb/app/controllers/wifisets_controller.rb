require 'socket'
require 'json'

class WifisetsController < ApplicationController
  before_action :set_wifiset, only: [:show, :edit, :update, :destroy]

  # GET /wifisets
  # GET /wifisets.json
  def index
    @wifisets = Wifiset.all
  end

  # GET /wifisets/1
  # GET /wifisets/1.json
  def show
  end

  # GET /wifisets/new
  def new
    @wifiset = Wifiset.new
  end

  # GET /wifisets/1/edit
  def edit
  end

  # POST /wifisets
  # POST /wifisets.json
  def create
    @wifiset = Wifiset.new(wifiset_params)
    puts @wifiset.ssid
    client = TCPSocket.new 'localhost', 9990
    makeSSIDjson = {:SSID => @wifiset.ssid, :PW => @wifiset.pass}
  client.puts JSON.generate(makeSSIDjson)
  while line = client.gets
    puts line
  end
    client.close

	#my_wifi_set = {:ssid => @wifiset.ssid}
    #puts my_wifi_set
	

    
    respond_to do |format|
      if @wifiset.save
        format.html { redirect_to @wifiset, notice: 'Wifiset was successfully created.' }
        format.json { render :show, status: :created, location: @wifiset }
      else
        format.html { render :new }
        format.json { render json: @wifiset.errors, status: :unprocessable_entity }
      end
    end
  end

  # PATCH/PUT /wifisets/1
  # PATCH/PUT /wifisets/1.json
  def update
    respond_to do |format|
      if @wifiset.update(wifiset_params)
        format.html { redirect_to @wifiset, notice: 'Wifiset was successfully updated.' }
        format.json { render :show, status: :ok, location: @wifiset }
      else
        format.html { render :edit }
        format.json { render json: @wifiset.errors, status: :unprocessable_entity }
      end
    end
  end

  # DELETE /wifisets/1
  # DELETE /wifisets/1.json
  def destroy
    @wifiset.destroy
    respond_to do |format|
      format.html { redirect_to wifisets_url, notice: 'Wifiset was successfully destroyed.' }
      format.json { head :no_content }
    end
  end

  private
    # Use callbacks to share common setup or constraints between actions.
    def set_wifiset
      @wifiset = Wifiset.find(params[:id])
    end

    # Never trust parameters from the scary internet, only allow the white list through.
    def wifiset_params
      params.require(:wifiset).permit(:ssid, :pass)
    end
end
