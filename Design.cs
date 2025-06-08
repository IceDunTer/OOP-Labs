using System;
using System.Diagnostics;
using System.Drawing;
using System.Windows.Forms;
using System.Collections.Generic;

namespace PerformanceMonitor
{
    public class MainForm : Form
    {
        private Button startButton;
        private Button stopButton;
        private ListView countersListView;
        private StatusStrip statusStrip;
        private ToolStripStatusLabel statusLabel;
        private System.Windows.Forms.Timer updateTimer;

        private List<PerformanceCounter> counters = new List<PerformanceCounter>();

        public MainForm()
        {
            InitializeComponent();
            InitializeCounters();
            SetupTimer();
        }

        private void InitializeComponent()
        {
            // Окно
            this.Text = "Мониторинг производительности";
            this.Size = new Size(600, 400);
            this.StartPosition = FormStartPosition.CenterScreen;

            // Кнопка "Старт"
            startButton = new Button
            {
                Text = "Старт",
                Location = new Point(20, 20),
                Size = new Size(80, 30)
            };
            startButton.Click += StartButton_Click;

            // Кнопка "Стоп"
            stopButton = new Button
            {
                Text = "Стоп",
                Location = new Point(110, 20),
                Size = new Size(80, 30),
                Enabled = false
            };
            stopButton.Click += StopButton_Click;

            // Счетчики
            countersListView = new ListView
            {
                View = View.Details,
                Location = new Point(20, 70),
                Size = new Size(540, 250),
                GridLines = true,
            };
            countersListView.Columns.Add("Счётчик", 300);
            countersListView.Columns.Add("Значение", 100);
            countersListView.Columns.Add("Единицы", 100);

            // Статусная строка
            statusStrip = new StatusStrip();
            statusLabel = new ToolStripStatusLabel { Text = "Готово" };
            statusStrip.Items.Add(statusLabel);

            this.Controls.Add(startButton);
            this.Controls.Add(stopButton);
            this.Controls.Add(countersListView);
            this.Controls.Add(statusStrip);
        }

        private void StartButton_Click(object sender, EventArgs e)
        {
            updateTimer.Start();
            startButton.Enabled = false;
            stopButton.Enabled = true;
            statusLabel.Text = "Мониторинг запущен...";
        }

        private void StopButton_Click(object sender, EventArgs e)
        {
            updateTimer.Stop();
            startButton.Enabled = true;
            stopButton.Enabled = false;
            statusLabel.Text = "Мониторинг остановлен";
        }

        private void InitializeCounters()
        {
            counters.Add(new PerformanceCounter("Processor", "% Processor Time", "_Total"));
            counters.Add(new PerformanceCounter("Memory", "Available MBytes"));

            foreach (var counter in counters)
                counter.NextValue();
        }

        private void SetupTimer()
        {
            updateTimer = new System.Windows.Forms.Timer
            {
                Interval = 1000 
            };
            updateTimer.Tick += UpdateCounters;
        }

        private void UpdateCounters(object sender, EventArgs e)
        {
            countersListView.Items.Clear();

            foreach (var counter in counters)
            {
                float value = counter.NextValue();
                string unit = Unit(counter.CounterName);

                var item = new ListViewItem($"{counter.CategoryName} ({counter.InstanceName}) - {counter.CounterName}");
                item.SubItems.Add(Math.Round(value, 2).ToString());
                item.SubItems.Add(unit);
                countersListView.Items.Add(item);
            }

            statusLabel.Text = $"Обновлено: {DateTime.Now:T}";
        }

        private string Unit(string counterName)
        {
            return counterName switch
            {
                "% Processor Time" => "%",
                "Available MBytes" => "MB",
                _ => ""
            };
        }
    }
}
