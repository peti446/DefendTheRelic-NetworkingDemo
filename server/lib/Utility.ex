defmodule Utility do

  def packetToString(s) do
    <<_::binary-size(4), message::binary>> = s;
    if(not String.valid?(message)) do
      message = ""
    end
    message
  end


  def packetStringSplitter(s) do
    String.split(s, "-::-")
  end
end
