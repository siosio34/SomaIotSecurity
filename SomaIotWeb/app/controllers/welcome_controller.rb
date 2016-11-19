require 'json'

class WelcomeController < ApplicationController

  def index

  end

  def test
    my_hash = {:hello => "goodbye"}
    @test_json = my_hash.to_json.to_s.html_safe
  end


end
