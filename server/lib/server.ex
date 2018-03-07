defmodule Server do
  use Application

  def start(_type, _args) do
    GlobalSupervisor.start_link([])
  end
end
