class CreateWifisets < ActiveRecord::Migration
  def change
    create_table :wifisets do |t|
      t.string :ssid
      t.string :pass

      t.timestamps null: false
    end
  end
end
