class LoginController < ApplicationController

  def index

  end

  def auth
    @pass = params[:password]
    passjson = {:page_name => 'login', :admin_pw => @pass}
    client.puts JSON.generate(passjson)

    while (line = client.gets)
      my_hash = JSON.parse(line)
      if my_hash[verify] == "true"
        render 'welcome/index'
      else
        render 'login/index'
      end
    end
    client.close
  end


end
