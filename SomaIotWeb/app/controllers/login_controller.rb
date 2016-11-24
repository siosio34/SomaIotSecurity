require 'rubygems'
require 'socket'
require 'json'
require 'csv'

class LoginController < ApplicationController

  def index

  end

  def auth
    @pass = params[:password] # 로그인으로부터 password 파라미터를 받아온다.
    passjson = {:page_name => 'login', :admin_pw => @pass} # c core server로 보낼 json을 만듬.
    client = TCPSocket.new 'localhost', 9090 # tcp socket 9090 포트로 염
    client.puts JSON.generate(passjson) # json을 서버로 보냄
    response = client.read # 서버로 보냇을때 응답을 json 형식으로 받아옴.
    strip_json = response.strip # c 에서 받아온 \n 문자열을 짜름
    my_hash = JSON.parse(strip_json) # json 형태로 파싱함
    client.close # 소켓 닫음.

    if my_hash["verify"] == 'true' # 로그인 성공시
      render 'welcome/index' # 메인 메뉴로 이동
    else # 로그인 실패시
      render 'login/auth' # 다시로그인
    end

  end

end
