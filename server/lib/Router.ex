defmodule Router do
  use GenServer

  @pName :s6073335_Router

  def start_link() do
    GenServer.start_link(__MODULE__, :ok, [])
  end

  def init(:ok) do
    Process.register(self(), @pName)
    {:ok, []}
  end

  def stop(reason \\ :normal, timeout \\ :infinity) do
    GenServer.stop(@pName, reason, timeout)
  end

  def terminate(reason, state) do
    IO.puts "Router closed, reason #{inspect reason} with state #{inspect state}"
  end

  #Handle messages functions

end
