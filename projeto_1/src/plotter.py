import matplotlib.pyplot as plt
import csv
import sys

log_file = sys.argv[1]
graph_file_name = sys.argv[2]

with open(log_file) as csv_file:
    csv_reader = csv.reader(csv_file, delimiter=',')
    line_count = 0
    internal_temperature = []
    external_temperature = []
    reference_temperature = []
    ventoinha = []
    resistor = []
    start_time, finish_time = 0, 0

    for row in csv_reader:
        if line_count == 1:
            start_time = row[0]

        elif line_count > 0:
            internal_temperature.append(float(row[1]))
            external_temperature.append(float(row[2]))
            reference_temperature.append(float(row[3]))
            ventoinha.append(float(row[4]))
            resistor.append(float(row[5]))
            finish_time = row[0]

        line_count += 1

fig, axs = plt.subplots(2)

axs[0].plot(range(len(internal_temperature)), internal_temperature, 'r-')
axs[0].plot(range(len(external_temperature)), external_temperature, 'b-')
axs[0].plot(range(len(reference_temperature)), reference_temperature, 'g-')
axs[0].legend(['Internal', 'External', 'Reference'])
axs[0].set(ylabel="Temperature (°C)")
axs[0].set_title("Comportamento da temperatura interna do sistema")
axs[0].label_outer()

axs[1].plot(range(len(ventoinha)), ventoinha, 'b-')
axs[1].plot(range(len(resistor)), resistor, 'r-')
axs[1].legend(['Fan', 'Resistor'])
axs[1].set(xlabel="Time (s)", ylabel="Intensity (%)")
axs[1].set_title("Acionamento dos controladores de temperatura")

plt.text(-20, -35, "Started: " + start_time)
plt.text(170, -35, "Finished: " + finish_time)
plt.savefig(graph_file_name)
