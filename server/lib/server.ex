defmodule Server do
  use Application

  def start(_type, _args) do
    case GlobalSupervisor.start_link([]) do
      {:ok, pid} ->
        commandParse()
        {:ok, pid}
      {:error, e} ->
        IO.puts("Could not start server")
        {:error, e}
    end
  end

  def commandParse() do
    case IO.gets("> ") do
      "stop\n" ->
      Router.stop()
      {:ok, :normal}
      _->
        commandParse()
    end
  end
end
