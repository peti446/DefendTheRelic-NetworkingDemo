defmodule Utility do

  def packetToString(s) do
    <<_::binary-size(4),  message::binary>> = s;
    message
  end

  def add_header_to_str(msg) do
    msg = to_string(msg)
    <<:erlang.size(msg)::big-32>> <> msg
  end

  def networkStringSplitter(s) do
    String.split(s, "-::-")
  end

  def random_string(length) do
    :crypto.strong_rand_bytes(length) |> Base.encode64 |> binary_part(0, length)
  end

  def random_string_32encode(length) do
    :crypto.strong_rand_bytes(length) |> Base.encode32 |> binary_part(0, length)
  end
end
