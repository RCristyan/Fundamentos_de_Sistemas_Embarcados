import matplotlib.pyplot as plt
import csv

with open('../test_log.txt') as csv_file:
    csv_reader = csv.reader(csv_file, delimiter=',')
    line_count = 0
    internal_temperature = []
    external_temperature = []
    reference_temperature = []
    start_time, finish_time = 0, 0

    for row in csv_reader:
        if line_count == 1:
            start_time = row[0]

        elif line_count > 0:
            internal_temperature.append(float(row[1]))
            external_temperature.append(float(row[2]))
            reference_temperature.append(float(row[3]))
            finish_time = row[0]

        line_count += 1

plt.plot(range(len(internal_temperature)), internal_temperature, 'r-')
plt.plot(range(len(external_temperature)), external_temperature, 'b-')
plt.plot(range(len(reference_temperature)), reference_temperature, 'g-')

plt.legend(['Internal temp.', 'External temp.', 'Reference temp.'])
plt.ylabel("Temperature (°C)")
plt.xlabel("Time (s)")

plt.text(-20, 18, "Started: " + start_time)
plt.text(120, 18, "Finished: " + finish_time)
plt.savefig("grafico.png")
