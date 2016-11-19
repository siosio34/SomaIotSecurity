require 'json'

class WelcomeController < ApplicationController

  def index

  end

  def test
    my_hash = {:hello => "goodbye"}
    @test_json = JSON.generate(my_hash)
  end


end
